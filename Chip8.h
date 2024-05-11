#pragma once
#include <cstdint>
#include <fstream>

namespace npdib
{
	class Chip8
	{
	public:
		Chip8();
		~Chip8() = default;
		void load_program(const char* filename);
		void run() { m_halted = false; }
		void pause() { m_halted = true; }
		void tick();
		uint8_t* get_display_data() { return k_display_data; }
		uint8_t* get_sound_register() { return k_sound_register; }
		uint8_t* get_key_register() { return k_key_register; }

	private:
		enum class Nibble
		{
			OpCode,
			Second,
			Third,
			Fourth
		};

		bool m_halted;

		static constexpr uint32_t kClockSpeed = 1000000;
		uint32_t m_cycles;

		static constexpr uint8_t font[] = { 
			0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
			0x20, 0x60, 0x20, 0x20, 0x70, // 1
			0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
			0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
			0x90, 0x90, 0xF0, 0x10, 0x10, // 4
			0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
			0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
			0xF0, 0x10, 0x20, 0x40, 0x40, // 7
			0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
			0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
			0xF0, 0x90, 0xF0, 0x90, 0x90, // A
			0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
			0xF0, 0x80, 0x80, 0x80, 0xF0, // C
			0xE0, 0x90, 0x90, 0x90, 0xE0, // D
			0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
			0xF0, 0x80, 0xF0, 0x80, 0x80  // F
		};

		/*
		0x0000 - 0x0049:	UNUSED
		0x0050 - 0x009F:	FONT
		0x00A0 - 0x01FF:	UNUSED
		0x0200 - 0x0E9F:	PROGRAM DATA
		0x0EA0 - 0x0EAF:	REGISTERS V0-VF
		0x0EB0 - 0x0EBF:	OTHER REGISTERS
		0x0EC0 - 0x0EFF:	CALL STACK
		0x0F00 - 0x0FFF:	DISPLAY DATA
		*/
		uint8_t m_ram[4096];
		uint8_t* const k_fonts;
		uint8_t* const k_v_registers;
		uint8_t* const k_i_register; // 2 bytes
		uint8_t* const k_delay_register; // 1 byte
		uint8_t* const k_sound_register; // 1 byte
		uint8_t* const k_key_register; // 2 bytes
		uint8_t* const k_call_stack;
		uint8_t* const k_display_data;
		uint8_t* const k_program_data;

		uint8_t* m_stack_pointer;
		uint8_t* m_program_counter;
		
		void push(uint8_t data);
		uint8_t pop();

		uint16_t m_current_op;

		void fetch();
		void execute();

		uint8_t retrieve_nibble(Nibble nibble);
		uint8_t* get_display_data(uint8_t x, uint8_t y);

		// operations

		void machine_language_routine();
		void jump();
		void subroutine();
		void skip_if_equal();
		void skip_if_not_equal();
		void skip_if_equal_registers();
		void set_register();
		void add_value_to_register();
		void arithmetic_operations();

		void set();
		void bin_or();
		void bin_and();
		void logic_xor();
		void add();
		void sub_left();
		void sub_right();
		void shift_left();
		void shift_right();

		void skip_if_not_equal_registers();
		void set_index_register();
		void jump_with_offset();
		void random();
		void display();
		void skip_if_key();

		void misc();

		void set_register_to_delay();
		void set_delay_from_register();
		void set_sound();
		void add_to_index();
		void get_key();
		void font_character();
		void bcd_conversion();
		void store_memory();
		void load_memory();
	};

}