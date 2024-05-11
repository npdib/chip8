#include "Display.h"
#include <cstdio>
#include <algorithm>

namespace npdib
{
    Display::Display(uint8_t* const pixel_location)
        : m_display_data(pixel_location)
        , m_window(nullptr)
        , m_renderer(nullptr)
        , m_cell_size(std::min((kScreenWidth * 0.9) / kScreenColumns, (kScreenHeight * 0.9) / kScreenRows))
        , m_grid{ .x = (kScreenWidth - m_cell_size * kScreenColumns) / 2, .y = (kScreenHeight - m_cell_size * kScreenRows) / 2, .w = m_cell_size * kScreenColumns, .h = m_cell_size * kScreenRows }
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

        display();
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

    void Display::display()
    {
        // clear screen

        SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(m_renderer);

        draw_grid();

        SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0xFF);

        for (uint16_t byte_counter = 0; byte_counter < 256; ++byte_counter)
        {
            for (uint8_t bit_counter = 0; bit_counter < 8; ++bit_counter)
            {
                if (m_display_data[byte_counter] & (1 << (7 - bit_counter)))
                {
                    SDL_Rect pixel = {
                        .x = m_grid.x + m_cell_size * (((byte_counter % 8) << 3) + bit_counter),
                        .y = m_grid.y + m_cell_size * (byte_counter / 8),
                        .w = m_cell_size,
                        .h = m_cell_size
                    };

                    SDL_RenderFillRect(m_renderer, &pixel);
                }
            }
        }

        SDL_RenderPresent(m_renderer);
    }

    void Display::draw_grid()
    {
        SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(m_renderer, &m_grid);

        SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0xFF);

        SDL_RenderDrawRect(m_renderer, &m_grid);

        for (uint8_t x = 1; x < kScreenColumns; ++x)
        {
            SDL_RenderDrawLine(m_renderer, m_grid.x + x * (m_grid.w / kScreenColumns), m_grid.y, m_grid.x + x * (m_grid.w / kScreenColumns), m_grid.y + m_grid.h - 1);
        }

        for (uint8_t y = 1; y < kScreenRows; ++y)
        {
            SDL_RenderDrawLine(m_renderer, m_grid.x, m_grid.y + y * (m_grid.h / kScreenRows), m_grid.x + m_grid.w - 1, m_grid.y + y * (m_grid.h / kScreenRows));
        }
    }

}