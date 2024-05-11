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

		void draw(uint8_t character);

	private:
		SDL_Window* m_window;
		SDL_Renderer* m_renderer;

		// Define screen dimensions
		static constexpr uint16_t kScreenWidth = 800;
		static constexpr uint16_t kScreenHeight = 600;

		uint8_t* const m_display_data; // this is where the pixel data is
	};

}