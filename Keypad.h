#pragma once

#include <SDL.h>

namespace npdib
{
	class Keypad
	{
	public:
		Keypad(uint8_t* keys);
		~Keypad() = default;


		// this function checks to see if there has been any input and returns the value that has been pressed if there has and 255 if not. if a quit event is wanted, 254 is returned
		bool poll();
	private:
		uint8_t* m_key_states;
		const uint8_t* const m_keyboard_states;
	};
}