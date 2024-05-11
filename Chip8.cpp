#include "Chip8.h"
#include <cstring>

namespace npdib
{

	Chip8::Chip8()
		: m_halted(true)
		, k_v_registers(m_ram + 0x0EA0)
		, k_i_register(m_ram + 0x0EB0)
		, k_delay_register(m_ram + 0x0EB2)
		, k_sound_register(m_ram + 0x0EB3)
		, k_call_stack(m_ram + 0x0EC0)
		, k_display_data(m_ram + 0x0F00)
		, k_program_data(m_ram + 0x0200)
		, m_stack_pointer(k_call_stack)
		, m_program_counter(k_program_data)
	{
		std::memset(m_ram, 0, sizeof(m_ram));
		std::memcpy(m_ram + 0x0050, font, sizeof(font));
	}

	void Chip8::load_program(const char* filename)
	{
		std::ifstream infile(filename);
		if (!infile.good())
		{
			throw std::runtime_error("ROM not found\n");
		}

		// write to memory - adapted from https://bisqwit.iki.fi/jutut/kuvat/programming_examples/chip8/chip8.cc
		uint16_t offset = 0x0200;
		for (std::ifstream f(filename, std::ios::binary); f.good();)
		{
			m_ram[offset++] = f.get();
		}
	}

	void Chip8::tick()
	{
		if (m_halted)
			return;

		if (*k_delay_register)
			--(*k_delay_register);

		if (*k_sound_register)
			--(*k_sound_register);

		while (m_cycles < kClockSpeed / 60) // potentially this is doing too many operations each tick
		{
			fetch();
			execute();
		}

		m_cycles -= kClockSpeed / 60;
	}

	void Chip8::push(uint8_t data)
	{
		if (m_stack_pointer - m_ram < 0x0F00)
		{
			*m_stack_pointer = data;
			++m_stack_pointer;
		}
		else
		{
			while (1); // lock up as stack overflow
		}
	}

	uint8_t Chip8::pop(uint8_t data)
	{
		if (m_stack_pointer - m_ram > 0x0EC0)
		{
			--m_stack_pointer;
		}
		else
		{
			while (1); // lock up as stack underflow
		}

		return *m_stack_pointer;
	}

	void Chip8::fetch()
	{
		m_current_op = (*m_program_counter << 8) | *(m_program_counter + 1);
		m_program_counter += 2;
	}

	void Chip8::execute()
	{
		switch (retrieve_nibble(Nibble::OpCode))
		{
			case 0x00:
				clear_screen();
				break;
			case 0x01:
				jump();
				break;
			case 0x02:
			case 0x03:
			case 0x04:
			case 0x05:
			case 0x06:
				set_register();
				break;
			case 0x07:
				add_value_to_register();
				break;
			case 0x08:
			case 0x09:
			case 0x0A:
				set_index_register();
				break;
			case 0x0B:
			case 0x0C:
			case 0x0D:
				display();
				break;
			case 0x0E:
			case 0x0F:
			default:
				break;
		}
	}

	uint8_t Chip8::retrieve_nibble(Nibble nibble)
	{
		switch (nibble)
		{
			case Nibble::OpCode:
				return (m_current_op >> 12) & 0x0F;
			case Nibble::Second:
				return (m_current_op >> 8) & 0x0F;
			case Nibble::Third:
				return (m_current_op >> 4) & 0x0F;
			case Nibble::Fourth:
				return m_current_op & 0x0F;
			default:
				break;
		}

		return 0;
	}

	uint8_t* Chip8::get_display_data(uint8_t x, uint8_t y)
	{
		return k_display_data + (y * 8) + (x / 8); // 8 bytes per row, 
	}

	void Chip8::clear_screen()
	{
		std::memset(m_ram + 0x0F00, 0, 256);
		m_cycles += 109;
	}


	void Chip8::jump()
	{
		m_program_counter = m_ram + (m_current_op & 0x0FFF);
		m_cycles += 105;
	}

	void Chip8::set_register()
	{
		*(k_v_registers + retrieve_nibble(Nibble::Second)) = (retrieve_nibble(Nibble::Third) << 4) | retrieve_nibble(Nibble::Fourth);
		m_cycles += 27;
	}

	void Chip8::add_value_to_register()
	{
		*(k_v_registers + retrieve_nibble(Nibble::Second)) += (retrieve_nibble(Nibble::Third) << 4) | retrieve_nibble(Nibble::Fourth);
		m_cycles += 45;
	}

	void Chip8::set_index_register()
	{
		*k_i_register = retrieve_nibble(Nibble::Second);
		*(k_i_register + 1) = (retrieve_nibble(Nibble::Third) << 4) | retrieve_nibble(Nibble::Fourth);
		m_cycles += 55;
	}

	void Chip8::display()
	{
		const uint8_t x_start = *(k_v_registers + retrieve_nibble(Nibble::Second)) % 64;
		const uint8_t y_start = *(k_v_registers + retrieve_nibble(Nibble::Third)) % 32;
		uint8_t x = x_start;
		uint8_t y = y_start;


		k_v_registers[0x0F] = 0;

		for (uint8_t rows = 0; rows < retrieve_nibble(Nibble::Fourth); ++rows)
		{
			uint8_t data = *(m_ram + ((*k_i_register) << 8) + *(k_i_register + 1) + rows);

			for (uint8_t bit_counter = 0; bit_counter < 8; ++bit_counter)
			{
				const bool pixel_on = k_display_data[(y * 8) + (x / 8)] & (1 << (7 - (x % 8)));
				const bool flip = data & (1 << (7 - bit_counter));

				if (pixel_on && flip)
				{
					k_v_registers[0x0F] = 1;
					k_display_data[(y * 8) + (x / 8)] &= ~(1 << (7 - bit_counter));
				}

				if (!pixel_on && flip)
				{
					k_display_data[(y * 8) + (x / 8)] |= (1 << (7 - (x % 8)));
				}

				if (++x >= 64)
					break;
			}

			if (++y >= 32)
				break;

			x = x_start;
		}

		m_cycles += 22734;
	}
}