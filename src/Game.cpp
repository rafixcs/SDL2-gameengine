#include <iostream>
#include "../lib/glm/glm.hpp"
#include "./Game.h"
#include "./Entity.h"
#include "./Components/TransformComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/KeyboardControlComponents.h"
#include "./Components/WindowCollider.h"
#include "./AssetManager.h"
#include "./Map.h"
#include "./Collision.h"
#include "./Components/ColliderComponent.h"
#include "./Components/TextLabelComponent.h"
#include "./Components/ProjectileEmitterComponent.h"

EntityManager manager;
SDL_Renderer* Game::renderer;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Entity* player = NULL;
Map* map;

Game::Game() {
    this->isRunning = false;
}

Game::~Game() {
    //TODO: Destroctor
}

bool Game::getIsRunning() const {
    return this->isRunning;
}


void Game::Initialize(int width, int heigth) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    if (TTF_Init() != 0) {
        std::cerr << "Error initializing SDL TTF" << std::endl;
        return;
    }

    this->window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        heigth,
        SDL_WINDOW_SHOWN
    );

    if (!this->window) {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    this->renderer = SDL_CreateRenderer( window, -1, 0);
    
    if (!this->renderer) {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    this->isRunning = true;

    LoadLevel(1);

    return;
}

void Game::LoadLevel(int levelNumber) {
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

    std::string levelName = "Level" + std::to_string(levelNumber);
    lua.script_file("./assets/scripts/" + levelName + ".lua");

    /*********************************************/
    /* LOADS ASSETS FROM LUA CONFIG FILE         */
    /*********************************************/
    sol::table levelData = lua[levelName];
    sol::table levelAssets = levelData["assets"];

    unsigned int assetIndex = 0;
    while (true) {
        sol::optional<sol::table> existsAssetIndexNode = levelAssets[assetIndex];
        if (existsAssetIndexNode == sol::nullopt) {
            break;
        } else {
            sol::table asset = levelAssets[assetIndex];
            std::string assetType = asset["type"];
            if (assetType.compare("texture") == 0) {
                std::string assetId = asset["id"];
                std::string assetFile = asset["file"];
                assetManager->AddTexture(assetId, assetFile.c_str());
            }
        }
        assetIndex++;
    }

    /******************************************************************/
    /* LOADS ENTITIES AND COMPONENTS FROM LUA CONFIG FILE            */
    /****************************************************************/
    sol::table levelEntities = levelData["entities"];

    unsigned int entityIndex = 0;
    while(true) {
        sol::optional<sol::table> existsEntityIndexNode = levelEntities[entityIndex];
        if (existsEntityIndexNode == sol::nullopt) {
            break;
        } else {
            sol::table entity = levelEntities[entityIndex];
            
            std::string nameEntity = entity["name"];
            LayerType layerEntity = static_cast<LayerType>(static_cast<int>(entity["layer"]));
            sol::table components = entity["components"];
            
            // Creating a new entity
            Entity& newEntity(manager.AddEntity(nameEntity, layerEntity));

            // Add transform component
            sol::optional<sol::table> existTransform = components["transform"];
            if (existTransform != sol::nullopt) {
                newEntity.AddComponent<TransformComponent>(
                    static_cast<int>(components["transform"]["position"]["x"]),
                    static_cast<int>(components["transform"]["position"]["y"]),
                    static_cast<int>(components["transform"]["velocity"]["x"]),
                    static_cast<int>(components["transform"]["velocity"]["y"]),
                    static_cast<int>(components["transform"]["width"]),
                    static_cast<int>(components["transform"]["height"]),
                    static_cast<int>(components["transform"]["scale"])
                );
            }

            sol::optional<sol::table> existSprite = components["sprite"];
            if (existSprite != sol::nullopt) {
                std::string textureId = components["sprite"]["textureAssetId"];
                bool isAnimated = components["sprite"]["animated"];
                if (isAnimated) {
                    newEntity.AddComponent<SpriteComponent>(
                        textureId.c_str(),
                        static_cast<int>(components["sprite"]["frameCount"]),
                        static_cast<int>(components["sprite"]["animationSpeed"]),
                        static_cast<bool>(components["sprite"]["hasDirections"]),
                        static_cast<bool>(components["sprite"]["fixed"])
                    );
                } else {
                    newEntity.AddComponent<SpriteComponent>(textureId.c_str());
                }
            }

            sol::optional<sol::table> existCollider = components["collider"];
            if (existCollider != sol::nullopt) {
                std::string colliderTag  = components["collider"]["tag"];
                newEntity.AddComponent<ColliderComponent>(
                    colliderTag,
                    static_cast<int>(components["transform"]["position"]["x"]),
                    static_cast<int>(components["transform"]["position"]["y"]),
                    static_cast<int>(components["transform"]["width"]),
                    static_cast<int>(components["transform"]["height"])
                );
            }

            sol::optional<sol::table> existInput = components["input"];
            if (existInput != sol::nullopt) {
                sol::optional<sol::table> existsKeyboardInputComponent = components["input"]["keyboard"];
                if (existsKeyboardInputComponent != sol::nullopt) {
                    std::string upkey = components["input"]["keyboard"]["up"];
                    std::string rightkey = components["input"]["keyboard"]["right"];
                    std::string downkey = components["input"]["keyboard"]["down"];
                    std::string leftkey = components["input"]["keyboard"]["left"];
                    std::string shootkey = components["input"]["keyboard"]["shoot"];
                    newEntity.AddComponent<KeyboardControlComponent>(upkey, downkey, rightkey, leftkey, shootkey);
                }
            } 

            sol::optional<sol::table> existProjectileEmitter = components["projectileEmitter"];
            if (existProjectileEmitter != sol::nullopt) {
                 int parentEntityXPos = entity["components"]["transform"]["position"]["x"];
                int parentEntityYPos = entity["components"]["transform"]["position"]["y"];
                int parentEntityWidth = entity["components"]["transform"]["width"];
                int parentEntityHeight = entity["components"]["transform"]["height"];
                int projectileWidth = entity["components"]["projectileEmitter"]["width"];
                int projectileHeight = entity["components"]["projectileEmitter"]["height"];
                int projectileSpeed = entity["components"]["projectileEmitter"]["speed"];
                int projectileRange = entity["components"]["projectileEmitter"]["range"];
                int projectileAngle = entity["components"]["projectileEmitter"]["angle"];
                bool projectileShouldLoop = entity["components"]["projectileEmitter"]["shouldLoop"];
                std::string textureAssetId = entity["components"]["projectileEmitter"]["textureAssetId"];
                Entity& projectile(manager.AddEntity("projectile", PROJECTILE_LAYER));
                projectile.AddComponent<TransformComponent>(
                    parentEntityXPos + (parentEntityWidth / 2),
                    parentEntityYPos + (parentEntityHeight / 2),
                    0,
                    0,
                    projectileWidth,
                    projectileHeight,
                    1
                );
                projectile.AddComponent<SpriteComponent>(textureAssetId.c_str());
                projectile.AddComponent<ProjectileEmitterComponent>(
                    projectileSpeed,
                    projectileAngle,
                    projectileRange,
                    projectileShouldLoop
                );
                projectile.AddComponent<ColliderComponent>(
                    "PROJECTILE",
                    parentEntityXPos,
                    parentEntityYPos,
                    projectileWidth,
                    projectileHeight
                );
            } 

            sol::optional<sol::table> existTextLabel = components["textLabel"];
            if (existTextLabel !=  sol::nullopt) {
                // TODO:...
            }
        }
        entityIndex++;
    }

    player = manager.GetEntityByName("player");

    /*********************************************/
    /* LOADS MAP FROM LUA CONFIG FILE            */
    /*********************************************/
    sol::table levelMap = levelData["map"];
    std::string mapTextureId = levelMap["textureAssetId"];
    std::string mapFile = levelMap["file"];

    map = new Map(
        mapTextureId,
        static_cast<int>(levelMap["scale"]),
        static_cast<int>(levelMap["tileSize"])
    );

    map->LoadMap(
        mapFile,
        static_cast<int>(levelMap["mapSizeX"]),
        static_cast<int>(levelMap["mapSizeY"])
    );
}

void Game::HandleCameraMovement() {
    if (player) {
        TransformComponent* mainPlayerTransform = player->GetComponent<TransformComponent>();

        camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
        camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

        camera.x = camera.x < 0 ? 0 : camera.x;     camera.x = camera.x > camera.w ? camera.w : camera.x;
        camera.y = camera.y < 0 ? 0 : camera.y;     camera.y = camera.y > camera.h ? camera.h : camera.y;
    }
}

void Game::CheckCollisions() {
    /*std::string collisionTagType =  manager.CheckEntityCollisions(player);
    if (collisionTagType.compare("enemy") == 0) {
        // TODO: do something when collision is identified with an enemy
        //isRunning = false;
        std::cout << "Got collision with enemy!" << std::endl;
    }*/

    CollisionType _collisionType = manager.CheckCollisions();
    if (_collisionType == PLAYER_ENEMY_COLLISION || _collisionType == PLAYER_PROJECTILE_COLLISION) {
        ProcessGameOver();
    }
    if (_collisionType == PLAYER_LEVEL_COMPLETE_COLLISION) {
        ProcessNextLevel(1);
    }

}

void Game::ProcessNextLevel(int levelNumber) {
    std::cout << "Next level" << std::endl;
    isRunning = false;
}

void Game::ProcessGameOver() {
    std::cout << "Game over" << std::endl;
    isRunning = false;
}

void Game::ProcessInput() {
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT: {
        this->isRunning = false;
        break;
    }
    case SDL_KEYDOWN: {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            this->isRunning = false;
        }
    }

    default: break;
    }
}

void Game::Update() {
    // Wait until 16ms has ellapsed since the last frame
    
    // Using while loop waste CPU usage
    //**while(!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TARGET_TIME));**
    
    // More eficient way to wait for the right time step
    int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - this->ticksLastFrame);
    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME)
        SDL_Delay(timeToWait);

    // Delta time is the difference in ticks from last frame converted to seconds
    float deltaTime = (SDL_GetTicks() - this->ticksLastFrame) / 1000.0f;

    // Sets the new ticks for the current frame to be used in the next pass
    this->ticksLastFrame = SDL_GetTicks();

    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

    // Here we call the manager.update to update all entities as function of deltaTime
    manager.Update(deltaTime);
    
    HandleCameraMovement();
    CheckCollisions();
    
}

void Game::Render() {
    SDL_SetRenderDrawColor(this->renderer, 21, 21, 21, 255);
    SDL_RenderClear(this->renderer);

    // Here we call the manager.render to render all entities
    manager.Render();

    SDL_RenderPresent(this->renderer);

}

void Game::Destroy() {
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}
