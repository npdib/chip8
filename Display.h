#pragma once

#include <cstdint>
#include <SDL.h>

class Display
{
public:
	Display(uint8_t* pixel_location);
	~Display();

	void draw();

private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

	uint8_t* const m_display_data; // this is where the pixel data is
};

