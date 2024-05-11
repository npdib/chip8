#include "Keypad.h"

namespace npdib
{
	Keypad::Keypad(uint8_t* keys)
		: m_key_states(reinterpret_cast<uint16_t*>(keys))
	{
	}
	bool Keypad::poll()
	{
		SDL_Event ev;
		if (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT)
			{
				return true;
			}
			else if (ev.type == SDL_KEYDOWN)
			{
				switch (ev.key.keysym.scancode)
				{
					case SDL_SCANCODE_ESCAPE:
						return true;
					case SDL_SCANCODE_1:
						*m_key_states |= (1 << 0x01);
						break;
					case SDL_SCANCODE_2:
						*m_key_states |= (1 << 0x02);
						break;
					case SDL_SCANCODE_3:
						*m_key_states |= (1 << 0x03);
						break;
					case SDL_SCANCODE_4:
						*m_key_states |= (1 << 0x0C);
						break;
					case SDL_SCANCODE_Q:
						*m_key_states |= (1 << 0x04);
						break;
					case SDL_SCANCODE_W:
						*m_key_states |= (1 << 0x05);
						break;
					case SDL_SCANCODE_E:
						*m_key_states |= (1 << 0x06);
						break;
					case SDL_SCANCODE_R:
						*m_key_states |= (1 << 0x0D);
						break;
					case SDL_SCANCODE_A:
						*m_key_states |= (1 << 0x07);
						break;
					case SDL_SCANCODE_S:
						*m_key_states |= (1 << 0x08);
						break;
					case SDL_SCANCODE_D:
						*m_key_states |= (1 << 0x09);
						break;
					case SDL_SCANCODE_F:
						*m_key_states |= (1 << 0x0E);
						break;
					case SDL_SCANCODE_Z:
						*m_key_states |= (1 << 0x0A);
						break;
					case SDL_SCANCODE_X:
						*m_key_states |= (1 << 0x00);
						break;
					case SDL_SCANCODE_C:
						*m_key_states |= (1 << 0x0B);
						break;
					case SDL_SCANCODE_V:
						*m_key_states |= (1 << 0x0F);
						break;
					default:
						break;
				}
			}
			else if (ev.type == SDL_KEYUP)
			{
				switch (ev.key.keysym.scancode)
				{
					case SDL_SCANCODE_1:
						*m_key_states &= ~(1 << 0x01);
						break;
					case SDL_SCANCODE_2:
						*m_key_states &= ~(1 << 0x02);
						break;
					case SDL_SCANCODE_3:
						*m_key_states &= ~(1 << 0x03);
						break;
					case SDL_SCANCODE_4:
						*m_key_states &= ~(1 << 0x0C);
						break;
					case SDL_SCANCODE_Q:
						*m_key_states &= ~(1 << 0x04);
						break;
					case SDL_SCANCODE_W:
						*m_key_states &= ~(1 << 0x05);
						break;
					case SDL_SCANCODE_E:
						*m_key_states &= ~(1 << 0x06);
						break;
					case SDL_SCANCODE_R:
						*m_key_states &= ~(1 << 0x0D);
						break;
					case SDL_SCANCODE_A:
						*m_key_states &= ~(1 << 0x07);
						break;
					case SDL_SCANCODE_S:
						*m_key_states &= ~(1 << 0x08);
						break;
					case SDL_SCANCODE_D:
						*m_key_states &= ~(1 << 0x09);
						break;
					case SDL_SCANCODE_F:
						*m_key_states &= ~(1 << 0x0E);
						break;
					case SDL_SCANCODE_Z:
						*m_key_states &= ~(1 << 0x0A);
						break;
					case SDL_SCANCODE_X:
						*m_key_states &= ~(1 << 0x00);
						break;
					case SDL_SCANCODE_C:
						*m_key_states &= ~(1 << 0x0B);
						break;
					case SDL_SCANCODE_V:
						*m_key_states &= ~(1 << 0x0F);
						break;
					default:
						break;
				}
			}
		}

		return false;
	}
}