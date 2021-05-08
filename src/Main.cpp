#include "./Constants.h"
#include "./Game.h"

int main(int argc, char *argv[]) {
    Game game;

    game.Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

    while(game.getIsRunning()) {
        game.ProcessInput();
        game.Update();
        game.Render();
    }

    game.Destroy();

    return 0;
}