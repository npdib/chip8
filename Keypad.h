#pragma once

#include <SDL.h>

namespace npdib
{
	class Keypad
	{
	public:
		Keypad() = default;
		~Keypad() = default;


		// this function checks to see if there has been any input and returns the value that has been pressed if there has and 255 if not. if a quit event is wanted, 254 is returned
		uint8_t poll();
	private:
	};
}