#include "Chip8.h"
#include <cstring>

namespace npdib
{

	Chip8::Chip8()
		: stack_pointer(ram + 0x0EB0)
		, delay(0)
		, sound(0)
	{
		std::memcpy(ram + 0x0050, font, sizeof(font));
	}

	Chip8::~Chip8()
	{
	}

	void Chip8::load_program(uint8_t* program)
	{
		std::memcpy(ram + 0x0200, program, 0x0E00);
	}

	void Chip8::run()
	{
	}

	void Chip8::push(uint8_t data)
	{
		if (stack_pointer - ram < 0x0F00)
		{
			*stack_pointer = data;
			++stack_pointer;
		}
		else
		{
			while (1); // lock up as stack overflow
		}
	}

	uint8_t Chip8::pop(uint8_t data)
	{
		if (stack_pointer - ram > 0x0EB0)
		{
			--stack_pointer;
		}
		else
		{
			while (1); // lock up as stack underflow
		}

		return *stack_pointer;
	}

}