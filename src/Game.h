#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SLD2/SLD_image.h>
#include <SLD2/SLD_ttf.h>

class Game {
    private:
        bool isRunning;
        SDL_Window *window;
        SDL_Renderer *renderer;
    public:
        Game();
        ~Game();
        
        void Initialize(int width, int height);
        void ProcessInput();
        void Update();
        void Render();
        void Destroy();
        
        bool getIsRunning() const;
        
};

#endif