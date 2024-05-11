#include "Display.h"
#include <cstdio>
#include <algorithm>

namespace npdib
{
    Display::Display(uint8_t* const pixel_location)
        : m_display_data(pixel_location)
        , m_window(nullptr)
        , m_renderer(nullptr)
    {
        // Create window
        m_window = SDL_CreateWindow("Basic C SDL project",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            kScreenWidth, kScreenHeight,
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

    void Display::draw(uint8_t character)
    {
        SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(m_renderer);

        SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0xFF);


        // Declare rect of square
        SDL_Rect squareRect;

        // Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
        squareRect.w = std::min(kScreenWidth, kScreenHeight) / 2;
        squareRect.h = std::min(kScreenWidth, kScreenHeight) / 2;

        // Square position: In the middle of the screen
        squareRect.x = kScreenWidth / 2 - squareRect.w / 2;
        squareRect.y = kScreenHeight / 2 - squareRect.h / 2;


        SDL_RenderDrawRect(m_renderer, &squareRect);

        SDL_RenderPresent(m_renderer);
    }
}