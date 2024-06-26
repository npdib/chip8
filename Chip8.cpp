#include "Chip8.h"
#include <cstring>
#include <iostream>

namespace npdib
{

	Chip8::Chip8()
		: m_halted(true)
		, k_fonts(m_ram + 0x0050)
		, k_v_registers(m_ram + 0x0EA0)
		, k_i_register(m_ram + 0x0EB0)
		, k_delay_register(m_ram + 0x0EB2)
		, k_sound_register(m_ram + 0x0EB3)
		, k_key_register(m_ram + 0x0EB4)
		, k_call_stack(m_ram + 0x0EC0)
		, k_display_data(m_ram + 0x0F00)
		, k_program_data(m_ram + 0x0200)
		, m_stack_pointer(k_call_stack)
		, m_program_counter(k_program_data)
		, key_held(false)
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

	uint8_t Chip8::pop()
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
				machine_language_routine();
				break;
			case 0x01:
				jump();
				break;
			case 0x02:
				subroutine();
				break;
			case 0x03:
				skip_if_equal();
				break;
			case 0x04:
				skip_if_not_equal();
				break;
			case 0x05:
				skip_if_equal_registers();
				break;
			case 0x06:
				set_register();
				break;
			case 0x07:
				add_value_to_register();
				break;
			case 0x08:
				arithmetic_operations();
				break;
			case 0x09:
				skip_if_not_equal_registers();
				break;
			case 0x0A:
				set_index_register();
				break;
			case 0x0B:
				jump_with_offset();
				break;
			case 0x0C:
				random();
				break;
			case 0x0D:
				display();
				break;
			case 0x0E:
				skip_if_key();
				break;
			case 0x0F:
				misc();
				break;
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

	void Chip8::machine_language_routine()
	{
		switch (m_current_op & 0x0FFF)
		{
			case 0x0E0:
				std::memset(m_ram + 0x0F00, 0, 256);
				m_cycles += 109;
				break;
			case 0x0EE:
			{
				uint16_t pc = 0;
				pc |= pop();
				pc |= (pop() << 8);
				m_program_counter = m_ram + pc;
				m_cycles += 105;
				break;
			}
			default:
				break;
		}
	}


	void Chip8::jump()
	{
		m_program_counter = m_ram + (m_current_op & 0x0FFF);
		m_cycles += 105;
	}

	void Chip8::subroutine()
	{
		const uint16_t pc = m_program_counter - m_ram;
		push(pc >> 8);
		push(pc & 0xFF);
		m_program_counter = m_ram + (m_current_op & 0x0FFF);
		m_cycles += 105;
	}

	void Chip8::skip_if_equal()
	{
		if (*(k_v_registers + retrieve_nibble(Nibble::Second)) == ((retrieve_nibble(Nibble::Third) << 4) | retrieve_nibble(Nibble::Fourth)))
		{
			m_program_counter += 2;
		}
		m_cycles += 55;
	}

	void Chip8::skip_if_not_equal()
	{
		if (*(k_v_registers + retrieve_nibble(Nibble::Second)) != ((retrieve_nibble(Nibble::Third) << 4) | retrieve_nibble(Nibble::Fourth)))
		{
			m_program_counter += 2;
		}
		m_cycles += 55;
	}

	void Chip8::skip_if_equal_registers()
	{
		if (*(k_v_registers + retrieve_nibble(Nibble::Second)) == *(k_v_registers + retrieve_nibble(Nibble::Third)))
		{
			m_program_counter += 2;
		}
		m_cycles += 73;
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

	void Chip8::arithmetic_operations()
	{
		switch (m_current_op & 0x0F)
		{
			case 0x00:
				set();
				break;
			case 0x01:
				bin_or();
				break;
			case 0x02:
				bin_and();
				break;
			case 0x03:
				logic_xor();
				break;
			case 0x04:
				add();
				break;
			case 0x05:
				sub_left();
				break;
			case 0x06:
				shift_right(); // contentious
				break;
			case 0x07:
				sub_right();
				break;
			case 0x0E:
				shift_left(); // contentious
				break;
			default:
				break;
		}
		m_cycles += 200;
	}

	void Chip8::set()
	{
		*(k_v_registers + retrieve_nibble(Nibble::Second)) = *(k_v_registers + retrieve_nibble(Nibble::Third));
	}

	void Chip8::bin_or()
	{
		*(k_v_registers + retrieve_nibble(Nibble::Second)) |= *(k_v_registers + retrieve_nibble(Nibble::Third));
	}

	void Chip8::bin_and()
	{
		*(k_v_registers + retrieve_nibble(Nibble::Second)) &= *(k_v_registers + retrieve_nibble(Nibble::Third));
	}

	void Chip8::logic_xor()
	{
		*(k_v_registers + retrieve_nibble(Nibble::Second)) ^= *(k_v_registers + retrieve_nibble(Nibble::Third));
	}

	void Chip8::add()
	{
		uint16_t sum = *(k_v_registers + retrieve_nibble(Nibble::Second)) + *(k_v_registers + retrieve_nibble(Nibble::Third));
		*(k_v_registers + retrieve_nibble(Nibble::Second)) = sum & 0xFF;
		k_v_registers[0x0F] = (sum & 0xFF00) ? 1 : 0;
	}

	void Chip8::sub_left()
	{
		uint8_t temp_flag = (k_v_registers[retrieve_nibble(Nibble::Second)] >= k_v_registers[retrieve_nibble(Nibble::Third)]) ? 1 : 0;
		k_v_registers[retrieve_nibble(Nibble::Second)] -= k_v_registers[retrieve_nibble(Nibble::Third)];

		k_v_registers[0x0F] = temp_flag;
	}

	void Chip8::sub_right()
	{
		uint8_t temp_flag = (k_v_registers[retrieve_nibble(Nibble::Third)] >= k_v_registers[retrieve_nibble(Nibble::Second)]) ? 1 : 0;

		k_v_registers[retrieve_nibble(Nibble::Second)] = k_v_registers[retrieve_nibble(Nibble::Third)] - k_v_registers[retrieve_nibble(Nibble::Second)];
		k_v_registers[0x0F] = temp_flag;
	}

	void Chip8::shift_left()
	{
		uint8_t temp_flag = (k_v_registers[retrieve_nibble(Nibble::Second)] & 0x70) >> 7;
		k_v_registers[retrieve_nibble(Nibble::Second)] <<= 1;
		k_v_registers[0x0F] = temp_flag;
	}

	void Chip8::shift_right()
	{
		uint8_t temp_flag = k_v_registers[retrieve_nibble(Nibble::Second)] & 0x01;
		k_v_registers[retrieve_nibble(Nibble::Second)] >>= 1;
		k_v_registers[0x0F] = temp_flag;
	}

	void Chip8::skip_if_not_equal_registers()
	{
		if (*(k_v_registers + retrieve_nibble(Nibble::Second)) != *(k_v_registers + retrieve_nibble(Nibble::Third)))
		{
			m_program_counter += 2;
		}
		m_cycles += 73;
	}

	void Chip8::set_index_register()
	{
		*k_i_register = retrieve_nibble(Nibble::Second);
		*(k_i_register + 1) = (retrieve_nibble(Nibble::Third) << 4) | retrieve_nibble(Nibble::Fourth);
		m_cycles += 55;
	}

	void Chip8::jump_with_offset()
	{
		uint16_t address = k_v_registers[retrieve_nibble(Nibble::Second)] + ((retrieve_nibble(Nibble::Second) << 8) | (retrieve_nibble(Nibble::Third) << 4) | retrieve_nibble(Nibble::Fourth));
		m_program_counter = m_ram + address;
		m_cycles += 105;
	}

	void Chip8::random()
	{
		uint8_t random = std::rand() & 0xFF;
		random &= ((retrieve_nibble(Nibble::Third) << 4) | retrieve_nibble(Nibble::Fourth));
		*(k_v_registers + retrieve_nibble(Nibble::Second)) = random;
		m_cycles += 164;
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
					k_display_data[(y * 8) + (x / 8)] &= ~(1 << (7 - (x % 8)));
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
	void Chip8::skip_if_key()
	{
		uint16_t keys = (*k_key_register << 8) | *(k_key_register + 1);
		uint8_t key = k_v_registers[retrieve_nibble(Nibble::Second)];
		switch (m_current_op & 0xFF)
		{
		case 0x9E:
			if ((keys & (1 << key)) != 0)
			{
				m_program_counter += 2;
			}
			break;
		case 0xA1:
			if ((keys & (1 << key)) == 0)
			{
				m_program_counter += 2;
			}
			break;
		default:
			break;
		}

		m_cycles += 73;
	}
	void Chip8::misc()
	{
		switch (m_current_op & 0xFF)
		{
			case 0x07:
				set_register_to_delay();
				break;
			case 0x15:
				set_register_to_delay();
				break;
			case 0x18:
				set_sound();
				break;
			case 0x1E:
				add_to_index();
				break;
			case 0x0A:
				get_key();
				break;
			case 0x29:
				font_character();
				break;
			case 0x33:
				bcd_conversion();
				break;
			case 0x55:
				store_memory();
				break;
			case 0x65:
				load_memory();
				break;
			default:
				break;
		}
	}

	void Chip8::set_register_to_delay()
	{
		k_v_registers[retrieve_nibble(Nibble::Second)] = *k_delay_register;
		m_cycles += 45;
	}

	void Chip8::set_delay_from_register()
	{
		*k_delay_register = k_v_registers[retrieve_nibble(Nibble::Second)];
		m_cycles += 45;
	}

	void Chip8::set_sound()
	{
		*k_sound_register = k_v_registers[retrieve_nibble(Nibble::Second)];
		m_cycles += 45;
	}

	void Chip8::add_to_index()
	{
		uint16_t index = (*k_i_register << 8) | *(k_i_register + 1);
		index += k_v_registers[retrieve_nibble(Nibble::Second)];
		*k_i_register = index >> 8;
		*(k_i_register + 1) = index & 0xFF;

		k_v_registers[0x0F] = (index & 0xF000) ? 1 : 0;
		m_cycles += 86;
	}

	void Chip8::get_key()
	{
		uint16_t keys = (*k_key_register << 8) | *(k_key_register + 1);
		if (!keys)
		{
			if (key_held)
			{
				key_held = false;
			}
			else
			{
				m_program_counter -= 2;
			}
		}
		else
		{
			uint8_t key = 0;

			while (keys != 1)
			{
				keys >>= 1;
				++key;
			}

			k_v_registers[retrieve_nibble(Nibble::Second)] = key;
			key_held = true;

			m_program_counter -= 2;
		}

		m_cycles += kClockSpeed / 60;
	}

	void Chip8::font_character()
	{
		uint16_t address = k_fonts - m_ram + (retrieve_nibble(Nibble::Second) * 5);
		*k_i_register = address >> 8;
		*(k_i_register + 1) = address & 0xFF;
		m_cycles += 91;
	}

	void Chip8::bcd_conversion()
	{
		uint16_t address = (*k_i_register << 8) | *(k_i_register + 1);
		uint8_t value = k_v_registers[retrieve_nibble(Nibble::Second)];
		uint8_t digit1 = value / 100;
		uint8_t digit2 = (value / 10) % 10;
		uint8_t digit3 = value % 10;

		*(m_ram + address) = digit1;
		*(m_ram + address + 1) = digit2;
		*(m_ram + address + 2) = digit3;

		m_cycles += 364;
		m_cycles += 73 * (digit1 + digit2 + digit3);
	}

	void Chip8::store_memory()
	{
		uint8_t bytes = retrieve_nibble(Nibble::Second) + 1;
		uint16_t address = (*k_i_register << 8) | *(k_i_register + 1);

		for (uint8_t offset = 0; offset < bytes; ++offset)
		{
			*(m_ram + address + offset) = k_v_registers[offset];
			m_cycles += 64;
		}
		m_cycles += 64;
	}

	void Chip8::load_memory()
	{
		uint8_t bytes = retrieve_nibble(Nibble::Second) + 1;
		uint16_t address = (*k_i_register << 8) | *(k_i_register + 1);

		for (uint8_t offset = 0; offset < bytes; ++offset)
		{
			k_v_registers[offset] = *(m_ram + address + offset);
			m_cycles += 64;
		}
		m_cycles += 64;
	}
}