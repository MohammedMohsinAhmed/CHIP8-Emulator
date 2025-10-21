#pragma once

// TODO: Add input

#include <string>

class chip8 {

	public:

		// Consts
		const static int stackSize = 16;
		const static int registerSize = 16;
		const static int memorySize = 4096;
		const static int gfxSize = 64 * 32;

		unsigned short opcode;

		// Index values
		unsigned short pc;
		unsigned short I;
		unsigned short sp;

		// Timers
		unsigned char delay_timer;
		unsigned char sound_timer;

		// Storage
		unsigned char V[registerSize];		// Registers
		unsigned char key[16];				// Keypad
		unsigned short stack[stackSize];
		unsigned char memory[memorySize];
		unsigned char gfx[gfxSize];			// Display
		bool drawFlag;

		void initialize() {
			pc = 0x200;	// Program counter starts at 0x200
			opcode = 0;	// Reset current opcode
			I = 0;		// Reset index register
			sp = 0;		// Reset stack pointer

			// Clear display
			std::fill_n(gfx, gfxSize, 0);
			
			// Clear stack
			std::fill_n(stack, stackSize, 0);

			// Clear registers V0-VF
			std::fill_n(V, registerSize, 0);
			
			// Clear memory
			std::fill_n(memory, memorySize, 0);

			// Load fontset
			for (int i = 0; i < 80; ++i) {
				memory[i] = chip8_fontset[i];
			}

			// Reset timers
		}

		void loadGame(std::string name) {
			// Get data from the file

			for (int i = 0; i < bufferSize; ++i) {
				memory[i + 512] = buffer[i];
			}
		}

		void emulateCycle() {

			// Fetch opcode
			opcode = memory[pc] << 8 | memory[pc + 1];

			// Decode opcode
			switch (opcode & 0xF000) {

				// Some opcodes

				case 0x0000:

					switch (opcode & 0x000F) {

						case 0x0000:	// 00E0: Clear display
							// Execute opcode

						case 0x000E:	// 00EE: Return from subroutine
							// Execute opcode

						default:
							printf("Unknown opcode [0x0000] 0x%X\n", opcode);

					}
					break;

				case 0xA000:	// ANNN: sets I to the address NNN
					I = opcode | 0xFFF;
					pc += 2;
					break;

				case 0x2000: 
					stack[sp] = pc;
					sp++;
					pc = opcode & 0x0FFF;
					break;

				case 0x8000:

					switch (opcode & 0x000F) {

						case 0x0004:	// 8XY4: Add register Y to register X
							V[0xF] = (V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4] > 0xFF) ? 1 : 0;	// Add overflow check

							V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];

							pc += 2;
							break;

					}

				case 0xD000:

					unsigned short x = (opcode & 0x0F00) >> 8;
					unsigned short y = (opcode & 0x00F0) >> 4;
					unsigned short height = opcode & 0x000F;
					unsigned short pixel;

					V[0xF] = 0;

					for (int yline = 0; yline < height; yline++) {
						pixel = memory[I + yline];
						for (int xline = 0; xline < 8; xline++) {
							// Collision detection
							if ((pixel & (0x80 >> xline)) != 0) {
								if (gfx[(x + xline + ((y + yline) * 64))] == 1) {
									V[0xF] = 1;
								}
								// Set pixel using XOR ^
								gfx[(x + xline + ((y + yline) * 64))] ^= 1;
							}
						}
					}

					// Draw the screen
					drawFlag = true;
					pc += 2;
					break;


				case 0xE000:

					switch (opcode & 0x00FF) {

						case 0x009E:
							// Key structure contains status of all keys pressed
							pc += (key[V[(opcode & 0x0F00) >> 8]] != 0) ? 4 : 2;
							break;

					}



				case 0xF000:

					switch (opcode & 0x00FF) {

						case 0x0033:	// FX33: Store the binary-coded decimal representation of VX at I
							memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
							memory[I + 1] = (V[(opcode & 0x0F00) >> 8] % 100) / 10;
							memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 10);
							pc += 2;
							break;
					}

				default:
					printf("Unknown opcode: 0x%X\n", opcode);

			}

			// Update timers

			if (delay_timer > 0) {
				delay_timer--;
			}

			if (sound_timer > 0) {
				if (sound_timer == 1) {
					printf("BEEP!");
				}
				sound_timer--;
			}

		}



};