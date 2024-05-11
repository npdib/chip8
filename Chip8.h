#pragma once
#include <cstdint>

class Chip8
{
public:
	Chip8();
	~Chip8();
	void LoadProgram(uint8_t* program);
	uint8_t* get_display_data() { return ram + 0xF00; }

private:
	uint8_t ram[4096];
};

