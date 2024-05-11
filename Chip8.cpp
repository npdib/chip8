#include "Chip8.h"
#include <cstring>

namespace npdib
{

	Chip8::Chip8()
	{
	}

	Chip8::~Chip8()
	{
	}

	void Chip8::load_program(uint8_t* program)
	{
		std::memcpy(ram + 0x200, program, 0xE00);
	}

}