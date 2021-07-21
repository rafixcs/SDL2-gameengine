#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../lib/lua/sol.hpp"


class AssetManager;

class Game {
    private:
        bool isRunning;
        SDL_Window *window;
    public:
        Game();
        ~Game();
        
        void Initialize(int width, int height);
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();
        void LoadLevel(int levelNumber);
        void HandleCameraMovement();
        void CheckCollisions();
        void ProcessNextLevel(int levelNumber);
        void ProcessGameOver();

    public:
        bool getIsRunning() const;
        
    public:
        int ticksLastFrame = 0;
        static SDL_Renderer *renderer;
        static AssetManager* assetManager;
        static SDL_Event event;
        static SDL_Rect camera; 
};

#endif