#include "Chip8.h"
#include <cstring>

Chip8::Chip8()
{
}

Chip8::~Chip8()
{
}

void Chip8::LoadProgram(uint8_t* program)
{
	std::memcpy(ram + 0x200, program, 0xE00);
}
