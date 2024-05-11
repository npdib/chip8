#include "Keypad.h"

namespace npdib
{
	uint8_t Keypad::poll()
	{
		SDL_Event ev;
		if (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_QUIT)
			{
				return 254;
			}
			else if (ev.type == SDL_KEYDOWN)
			{
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:
					return 254;
				case SDL_SCANCODE_1:
					return 0x01;
				case SDL_SCANCODE_2:
					return 0x02;
				case SDL_SCANCODE_3:
					return 0x03;
				case SDL_SCANCODE_4:
					return 0x0C;
				case SDL_SCANCODE_Q:
					return 0x04;
				case SDL_SCANCODE_W:
					return 0x05;
				case SDL_SCANCODE_E:
					return 0x06;
				case SDL_SCANCODE_R:
					return 0x0D;
				case SDL_SCANCODE_A:
					return 0x07;
				case SDL_SCANCODE_S:
					return 0x08;
				case SDL_SCANCODE_D:
					return 0x09;
				case SDL_SCANCODE_F:
					return 0x0E;
				case SDL_SCANCODE_Z:
					return 0x0A;
				case SDL_SCANCODE_X:
					return 0x00;
				case SDL_SCANCODE_C:
					return 0x0B;
				case SDL_SCANCODE_V:
					return 0x0F;
				default:
					break;
				}
			}
		}

		return 255;
	}
}