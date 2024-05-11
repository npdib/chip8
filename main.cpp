#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>
#include "Display.h"
#include "Chip8.h"

int main(int argc, char* argv[])
{
    // Unused argc, argv
    (void)argc;
    (void)argv;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not be initialized!\n"
            "SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    Chip8 chip8;
    Display display(chip8.get_display_data());

    return 0;
}