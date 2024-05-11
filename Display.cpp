#include "Display.h"
#include <cstdio>

// Define MAX and MIN macros
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

// Define screen dimensions
#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600

Display::Display(uint8_t* const pixel_location)
    : m_display_data(pixel_location)
    , m_window(nullptr)
    , m_renderer(nullptr)
{
    // Create window
    m_window = SDL_CreateWindow("Basic C SDL project",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (!m_window)
    {
        printf("Window could not be created!\n"
            "SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        // Create renderer
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
        if (!m_renderer)
        {
            printf("Renderer could not be created!\n"
                "SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            // Initialize renderer color white for the background
            SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

            // Clear screen
            SDL_RenderClear(m_renderer);

            // Update screen
            SDL_RenderPresent(m_renderer);
        }
    }
}

Display::~Display()
{
    // Destroy renderer
    SDL_DestroyRenderer(m_renderer);

    // Destroy window
    SDL_DestroyWindow(m_window);

    // Quit SDL
    SDL_Quit();
}

void Display::draw()
{
}
