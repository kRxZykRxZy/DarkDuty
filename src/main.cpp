#include "graphics/opengl/GpuPreference.h"
#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>
#include "Game.h"

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    std::srand((unsigned)std::time(nullptr));
    Game game;
    if (!game.init()) return 1;
    game.run();
    game.shutdown();
    return 0;
}
