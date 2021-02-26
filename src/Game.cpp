#include <iostream>
#include "./Constants.h"
#include "./Game.h"

Game::Game() {
    this->isRunning = false;
}

Game::~Game() {
    //TODO: Destroctor
}

bool Game::getIsRunning() const {
    return this->isRunning;
}

float projectilePosX = 0.0f;
float projectilePosY = 0.0f;
float projectileVelX = 10.0f;
float projectileVelY = 10.0f;

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
    projectilePosX += projectileVelX;
    projectilePosY += projectileVelY;
}

void Game::Render() {
    SDL_SetRendererDrawColor(this->renderer, 21, 21, 21, 255);
    SDL_RenderClear(this->renderer);

    SDL_Rect proj {
        (int) projectilePosX,
        (int) projectilePosY,
        10,
        10
    };

    SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(this->renderer, &proj);

    SDL_RenderPresent(this->renderer);

}

void Game::Destroy() {
    SDL_DestroyRender(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}
