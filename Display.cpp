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



        // Declare rect of square
        SDL_Rect box;

        // Square dimensions: Half of the min(SCREEN_WIDTH, SCREEN_HEIGHT)
        box.w = std::min(kScreenWidth, kScreenHeight) / 2;
        box.h = std::min(kScreenWidth, kScreenHeight) / 2;

        // Square position: In the middle of the screen
        box.x = kScreenWidth / 2 - box.w / 2;
        box.y = kScreenHeight / 2 - box.h / 2;

        SDL_Rect highlight;

        highlight.w = box.w / 4;
        highlight.h = box.h / 4;

        switch (character)
        {
            case 0x00:
                highlight.x = box.x + highlight.w;
                highlight.y = box.y + 3 * highlight.h;
                break;
            case 0x01:
                highlight.x = box.x;
                highlight.y = box.y;
                break;
            case 0x02:
                highlight.x = box.x + highlight.w;
                highlight.y = box.y;
                break;
            case 0x03:
                highlight.x = box.x + 2 * highlight.w;
                highlight.y = box.y;
                break;
            case 0x04:
                highlight.x = box.x;
                highlight.y = box.y + highlight.h;
                break;
            case 0x05:
                highlight.x = box.x + highlight.w;
                highlight.y = box.y + highlight.h;
                break;
            case 0x06:
                highlight.x = box.x + 2 * highlight.w;
                highlight.y = box.y + highlight.h;
                break;
            case 0x07:
                highlight.x = box.x;
                highlight.y = box.y + 2 * highlight.h;
                break;
            case 0x08:
                highlight.x = box.x + highlight.w;
                highlight.y = box.y + 2 * highlight.h;
                break;
            case 0x09:
                highlight.x = box.x + 2 * highlight.w;
                highlight.y = box.y + 2 * highlight.h;
                break;
            case 0x0A:
                highlight.x = box.x;
                highlight.y = box.y + 3 * highlight.h;
                break;
            case 0x0B:
                highlight.x = box.x + 2 * highlight.w;
                highlight.y = box.y + 3 * highlight.h;
                break;
            case 0x0C:
                highlight.x = box.x + 3 * highlight.w;
                highlight.y = box.y;
                break;
            case 0x0D:
                highlight.x = box.x + 3 * highlight.w;
                highlight.y = box.y + highlight.h;
                break;
            case 0x0E:
                highlight.x = box.x + 3 * highlight.w;
                highlight.y = box.y + 2 * highlight.h;
                break;
            case 0x0F:
                highlight.x = box.x + 3 * highlight.w;
                highlight.y = box.y + 3 * highlight.h;
                break;
            default:
                break;
        }

        SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderDrawRect(m_renderer, &box);

        SDL_SetRenderDrawColor(m_renderer, 0xFF, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(m_renderer, &highlight);

        SDL_RenderPresent(m_renderer);
    }
}