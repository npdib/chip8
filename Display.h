#pragma once

#include <cstdint>
#include <SDL.h>

namespace npdib
{
	class Display
	{
	public:
		Display(uint8_t* pixel_location);
		~Display();

		void display();

	private:
		SDL_Window* m_window;
		SDL_Renderer* m_renderer;

		// Define screen dimensions
		static constexpr uint16_t kScreenWidth = 1024;
		static constexpr uint16_t kScreenHeight = 600;
		static constexpr uint8_t kScreenColumns = 64;
		static constexpr uint8_t kScreenRows = 32;

		uint8_t* const m_display_data; // this is where the pixel data is
		const uint16_t m_cell_size;
		const SDL_Rect m_grid;

		void draw_grid();
	};

}