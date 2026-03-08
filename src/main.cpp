#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "graphics/opengl/GpuPreference.h"
#define SDL_MAIN_HANDLED
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
