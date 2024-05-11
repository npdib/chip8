#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

#include "Display.h"
#include "Keypad.h"
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

    npdib::Chip8 chip8;
    npdib::Display display(chip8.get_display_data());
    npdib::Keypad keypad;

    while (true)
    {
        uint8_t key_ev = keypad.poll();

        if (key_ev == 255)
        {
            continue;
        }
        else if (key_ev == 254)
        {
            break;
        }
        else
        {
            display.draw();
        }
    }

    return 0;
}