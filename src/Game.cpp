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

EntityManager manager;
SDL_Renderer* Game::renderer;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
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

    LoadLevel(0);

    return;
}

Entity& player(manager.AddEntity("chopper", PLAYER_LAYER));

void Game::LoadLevel(int levelNumber) {
    /* Start including new assets to the assetManager list */     
    assetManager->AddTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
    assetManager->AddTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
    assetManager->AddTexture("radar-image", std::string("./assets/images/radar.png").c_str());
    assetManager->AddTexture("jungle-tiletexture", std::string("./assets/tilemaps/jungle.png").c_str());
    assetManager->AddTexture("heliport-image", std::string("./assets/images/heliport.png").c_str());
    assetManager->AddFont("charriot-font", std::string("./assets/fonts/charriot.ttf").c_str(), 14);

    map = new Map("jungle-tiletexture", 2, 32);
    map->LoadMap("./assets/tilemaps/jungle.map", 25, 20);

    /* Start including entities and also components to them */
    player.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
    player.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
    player.AddComponent<KeyboardControlComponent>("up", "down", "right", "left", "space");
    //player.AddComponent<WindowCollider>();
    player.AddComponent<ColliderComponent>("PLAYER", 240, 106, 32, 32);
    //player.GetComponent<ColliderComponent>()->debugMode = true;

    Entity& heliport(manager.AddEntity("helliport", OBSTACLE_LAYER));
    heliport.AddComponent<TransformComponent>(470, 420, 0, 0, 32, 32, 1);
    heliport.AddComponent<SpriteComponent>("heliport-image");
    heliport.AddComponent<ColliderComponent>("LEVEL_COMPLETE", 470, 420, 32, 32);
    
    Entity& tankEntity(manager.AddEntity("tank", ENEMY_LAYER));
    tankEntity.AddComponent<TransformComponent>(150, 495, 20, 0, 32, 32, 1);
    tankEntity.AddComponent<SpriteComponent>("tank-image");
    tankEntity.AddComponent<ColliderComponent>("ENEMY", 150, 495, 32, 32);
    //tankEntity.GetComponent<ColliderComponent>()->debugMode = true;

    Entity& radarEntity(manager.AddEntity("radar", UI_LAYER));
    radarEntity.AddComponent<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
    radarEntity.AddComponent<SpriteComponent>("radar-image", 8, 150, false, true);

    Entity& levelLabel(manager.AddEntity("LabelLevelName", UI_LAYER));
    levelLabel.AddComponent<TextLabelComponent>(10, 10, "First Level...", "charriot-font", WHITE_COLOR);
}

void Game::HandleCameraMovement() {
    TransformComponent* mainPlayerTransform = player.GetComponent<TransformComponent>();

    camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
    camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

    camera.x = camera.x < 0 ? 0 : camera.x;     camera.x = camera.x > camera.w ? camera.w : camera.x;
    camera.y = camera.y < 0 ? 0 : camera.y;     camera.y = camera.y > camera.h ? camera.h : camera.y;
}

void Game::CheckCollisions() {
    /*std::string collisionTagType =  manager.CheckEntityCollisions(player);
    if (collisionTagType.compare("enemy") == 0) {
        // TODO: do something when collision is identified with an enemy
        //isRunning = false;
        std::cout << "Got collision with enemy!" << std::endl;
    }*/

    CollisionType _collisionType = manager.CheckCollisions();
    if (_collisionType == PLAYER_ENEMY_COLLISION) {
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
