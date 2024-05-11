#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

#include "Display.h"
#include "Keypad.h"
#include "Chip8.h"

#define TICKS_IN_60HZ 16

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

    // create objects
    npdib::Chip8 chip8;
    npdib::Display display(chip8.get_display_data());
    npdib::Keypad keypad;

    chip8.load_program("roms/IBM Logo.ch8");


    chip8.run();
    uint64_t next_tick = 0;

    while (true)
    {
        if (SDL_GetTicks64() > next_tick) // 60Hz
        {
            next_tick = SDL_GetTicks() + TICKS_IN_60HZ; // update next tick point
            chip8.tick();
            display.display();
        }

        uint8_t key_ev = keypad.poll();

        if (key_ev == 254)
        {
            break;
        }
    }

    return 0;
}