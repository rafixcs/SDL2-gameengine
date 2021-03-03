#include <iostream>
#include "./Constants.h"
#include "./Game.h"
#include "../lib/glm/glm.hpp"

Game::Game() {
    this->isRunning = false;
}

Game::~Game() {
    //TODO: Destroctor
}

bool Game::getIsRunning() const {
    return this->isRunning;
}

glm::vec2 projPos = glm::vec2(0.0f, 0.0f);
glm::vec2 projVel = glm::vec2(20.0f, 20.0f);

void Game::Initialize(int width, int heigth) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    this->window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        heigth,
        SDL_WINDOW_BORDERLESS
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

    return;
}

void Game::ProcessInput() {
    SDL_Event event;
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

    // Use deltaTime to update my game objects
    projPos = glm::vec2(
        projPos.x + projVel.x * deltaTime,
        projPos.y + projVel.y * deltaTime
    );
    
}

void Game::Render() {
    SDL_SetRenderDrawColor(this->renderer, 21, 21, 21, 255);
    SDL_RenderClear(this->renderer);

    SDL_Rect proj {
        static_cast<int>(projPos.x),
        static_cast<int>(projPos.y),
        10,
        10
    };

    SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(this->renderer, &proj);

    SDL_RenderPresent(this->renderer);

}

void Game::Destroy() {
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}
