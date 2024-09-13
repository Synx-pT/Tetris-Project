// Copyright Paul Tröster
// Ü11 - Uni Freiburg

#include "Colors.h"
#include "Game.h"
#include "TerminalManager.h"
#include "Tetromino.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <utility>
#include <vector>

int main(int argc, char *argv[]) {

  int argValue = 0; // Default value

  char rotateLeft = 'j';
  char rotate180 = 'k';
  char rotateRight = 'l';

  // Parsing command line arguments
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "--rotate-left" && i + 1 < argc) {
      rotateLeft = argv[i + 1][0];
      ++i;
    } else if (std::string(argv[i]) == "--rotate-180" && i + 1 < argc) {
      rotate180 = argv[i + 1][0];
      ++i;
    } else if (std::string(argv[i]) == "--rotate-right" && i + 1 < argc) {
      rotateRight = argv[i + 1][0];
      ++i;
    } else {
      try {
        argValue = std::stoi(argv[i]);
      } catch (std::invalid_argument &e) {
        std::cerr << "Error: Argument must be an integer or a valid key option."
                  << std::endl;
        return 1;
      } catch (std::out_of_range &e) {
        std::cerr << "Error: Argument is out of range for an integer."
                  << std::endl;
        return 1;
      }
    }
  }

  // Check if there are too many arguments
  if (argc > 7) { // 1 for program name + 6 potential args for keys
    std::cerr << "Error: Too many arguments." << std::endl;
    std::cerr << "Usage: " << argv[0]
              << " [--rotate-left <char>] [--rotate-180 <char>] "
                 "[--rotate-right <char>] [int]"
              << std::endl;
    return 1;
  }

  // Initialize Terminal Manager with the init_list
  TerminalManager terminalManager(init_list);

  // Initialize Game
  Game game(terminalManager);

  // Set level/keys according to command line input
  game.setLevel(argValue);
  game.setRotationKeys(rotateLeft, rotate180, rotateRight);

  // Count the frames to know wether a tetromino should move down
  int frameCount = 0;

  while (!game.isStopped()) {
    char input = terminalManager.getUserInput().keycode_;

    game.handleInput(input);

    // update game state if not paused and q wasnt pressed
    if (!game.isPaused()) {

      frameCount++;

      game.update(terminalManager);

      // Check if the frameCount is greater then the move down event integer
      if (game.mdTetromino() < frameCount) {
        game.moveDown();
        frameCount = 0;
      }
    }

    // 16ms should equal about 60 frames per second
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }

  terminalManager.~TerminalManager();

  return 0;
}
