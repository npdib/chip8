#include "Keypad.h"
#include <iostream>

namespace npdib
{
	Keypad::Keypad(uint8_t* keys)
		: m_key_states(keys)
		, m_keyboard_states(SDL_GetKeyboardState(nullptr))
	{
	}
	bool Keypad::poll()
	{
		SDL_PumpEvents();
		uint16_t key_states = 0;
		key_states |= m_keyboard_states[SDL_SCANCODE_1] << 1;
		key_states |= m_keyboard_states[SDL_SCANCODE_2] << 2;
		key_states |= m_keyboard_states[SDL_SCANCODE_3] << 3;
		key_states |= m_keyboard_states[SDL_SCANCODE_4] << 12;
		key_states |= m_keyboard_states[SDL_SCANCODE_Q] << 4;
		key_states |= m_keyboard_states[SDL_SCANCODE_W] << 5;
		key_states |= m_keyboard_states[SDL_SCANCODE_E] << 6;
		key_states |= m_keyboard_states[SDL_SCANCODE_R] << 13;
		key_states |= m_keyboard_states[SDL_SCANCODE_A] << 7;
		key_states |= m_keyboard_states[SDL_SCANCODE_S] << 8;
		key_states |= m_keyboard_states[SDL_SCANCODE_D] << 9;
		key_states |= m_keyboard_states[SDL_SCANCODE_F] << 14;
		key_states |= m_keyboard_states[SDL_SCANCODE_Z] << 10;
		key_states |= m_keyboard_states[SDL_SCANCODE_X] << 0;
		key_states |= m_keyboard_states[SDL_SCANCODE_C] << 11;
		key_states |= m_keyboard_states[SDL_SCANCODE_V] << 15;

		SDL_Event ev;
		if (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT)
			{
				return true;
			}
			else if (ev.type == SDL_KEYDOWN)
			{
				if (ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				{
					return true;
				}
			}
		}

		*m_key_states = key_states >> 8;
		*(m_key_states + 1) = key_states & 0xFF;

		return false;
	}
}