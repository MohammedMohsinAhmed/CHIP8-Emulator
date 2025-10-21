#include "chip8.h"

chip8 myChip8;

int main(int argc, char** argv) {
	// Setup render system and register input callbacks
	setupGraphics();
	setupInput();

	// Initialize the Chip8 system and load the game into memory
	myChip8.initialize();
	myChip8.loadGame("pong");

	// Emulation loop
	for (;;) {

		// Emulate one cycle
		myChip8.emulateCycle();

		// If the draw flag is set, update the screen
		if (myChip8.drawFlag) {
			drawGraphics();
		}

		myChip8.setKeys();

	}

	return 0;

}