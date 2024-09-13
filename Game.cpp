// Copyright Paul Tröster
// Ü11 - Uni Freiburg

#include "Game.h"
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <string>

Game::Game(TerminalManager &terminalManager) {
  tetrisCount_ = 0;
  mdTetromino_ = 48;
  currentLevel_ = 0;
  paused_ = false;
  gameStop_ = false;
  score_ = 0;

  // Create the default board and draw the border
  board_ = std::vector<std::vector<int>>(20, std::vector<int>(10, cleanColor_));
  drawBorder(terminalManager);

  // Set random seed and tetromino type
  srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  nextTetromino_.reset(static_cast<TetrominoType>(rand() % 6));

  spawnTetromino();
}

void Game::update(TerminalManager &terminalManager) {
  clean(terminalManager); // Clean board/nextPiece

  increaseLevel();

  setGhostPiece(terminalManager); // Set/Draw the ghost piece

  drawTetromino(terminalManager);

  drawInfoPanel(terminalManager);

  drawBoard(terminalManager);

  terminalManager.refresh(); // Refresh the terminal manager

  if (checkTopOut()) {
    // If theres a top out, set false to stop running
    gameStop_ = true;
  }
}

// ____________________________________________________________________________

// #######
// DRAWING
// #######

// ____________________________________________________________________________

void Game::drawTetromino(TerminalManager &terminalManager) {

  // Create a const reference of shape
  const std::vector<std::vector<int>> &shape = currentTetromino_.getShape();

  // Iterate over the shape reference vector
  for (std::size_t y = 0; y < shape.size(); ++y) {
    for (std::size_t x = 0; x < shape[y].size(); ++x) {
      // If the value returns 0, there's no pixel to draw
      if (shape[y][x] != 0) {
        // Add the coordinates to the current value in the shape
        terminalManager.drawPixel(tetrominoX_ + x + borderSize_,
                                  tetrominoY_ + y, shape[y][x]);
      }
    }
  }
}

// ____________________________________________________________________________

void Game::drawGhostPiece(TerminalManager &terminalManager, int ghostY) {

  const std::vector<std::vector<int>> &shape = currentTetromino_.getShape();

  for (std::size_t y = 0; y < shape.size(); ++y) {
    for (std::size_t x = 0; x < shape[y].size(); ++x) {
      if (shape[y][x] != 0) {
        // Add the ghost y value to draw at the lowest possible place
        terminalManager.drawPixel(tetrominoX_ + x + borderSize_, ghostY + y,
                                  shape[y][x] +
                                      8); // +8 to differentiate ghost piece
      }
    }
  }
}

// ____________________________________________________________________________

void Game::drawInfoPanel(TerminalManager &terminalManager) {

  // NEXT PIECE

  const std::vector<std::vector<int>> &shape = nextTetromino_.getShape();

  terminalManager.drawString(3, 15, 0, "NEXT");

  for (std::size_t y = 0; y < shape.size(); ++y) {
    for (std::size_t x = 0; x < shape[y].size(); ++x) {
      if (shape[y][x] != 0) {
        terminalManager.drawPixel(15 + x, 4 + y, shape[y][x]);
      }
    }
  }

  // CURRENT LEVEL

  terminalManager.drawString(9, 15, 0, "LEVEL");

  std::string tc_str = std::to_string(currentLevel_);
  const char *cstr = tc_str.c_str();
  terminalManager.drawString(10, 15, 0, cstr);

  // SCORE

  terminalManager.drawString(14, 15, 0, "SCORE");

  std::string tc_str2 = std::to_string(score_);
  const char *cstr2 = tc_str2.c_str();
  terminalManager.drawString(15, 15, 0, cstr2);
}

// ____________________________________________________________________________

void Game::drawBorder(TerminalManager &terminalManager) {
  for (int y = 0; y < 20 + 1; y++) { // Added +1 for bottom border
    terminalManager.drawPixel(0, y, borderColor_);
    terminalManager.drawPixel(11, y, borderColor_);
  }

  for (int x = 0; x < 12; x++) {
    terminalManager.drawPixel(x, 20, borderColor_);
  }
}

// ____________________________________________________________________________

void Game::drawBoard(TerminalManager &terminalManager) {
  for (std::size_t y = 0; y < board_.size(); ++y) {
    for (std::size_t x = 0; x < board_[y].size(); ++x) {
      if (board_[y][x] != 0) {
        terminalManager.drawPixel(x + borderSize_, y, board_[y][x]);
      }
    }
  }
}

// ____________________________________________________________________________

void Game::clean(TerminalManager &terminalManager) {
  // Clean board
  for (int y = 0; y < 20; y++) {
    for (int x = 1; x < 11; x++) {
      terminalManager.drawPixel(x, y, cleanColor_);
    }
  }
  // Clean next
  for (int y = 0; y < 4; ++y) {
    for (int x = 0; x < 4; ++x) {
      terminalManager.drawPixel(15 + x, 4 + y, cleanColor_);
    }
  }
}

// ____________________________________________________________________________

// ############
// HANDLE INPUT
// ############

// ____________________________________________________________________________

void Game::handleInput(char input) {
  if (input == 'p') {
    paused_ = !paused_;
  } else if (input == 'a') {
    if (!paused_)
      moveLeft();
  } else if (input == 's') {
    if (!paused_)
      hardDrop();
  } else if (input == 'd') {
    if (!paused_)
      moveRight();
  } else if (input == 'w') {
    if (!paused_)
      moveDown();
  } else if (input == rotateLeftKey_) {
    if (!paused_)
      rotate(-1);
  } else if (input == rotate180Key_) {
    if (!paused_)
      rotate(2);
  } else if (input == rotateRightKey_) {
    if (!paused_)
      rotate(1);
  } else if (input == 's') {
    if (!paused_)
      moveDown();
  } else if (input == 'q') {
    gameStop_ = true;
  }
}

// ____________________________________________________________________________

void Game::moveDown() {
  if (!checkCollision(0, 1, 0)) {
    tetrominoY_++;
  } else {
    placeTetromino();
    spawnTetromino();
    clearFullLines();
  }
}

// ____________________________________________________________________________

// ################
// HELPER FUNCTIONS
// ################

// ____________________________________________________________________________

void Game::clearFullLines() {

  // Count how many tetrises are cleared at once in order to set the score
  int kCount = 0;

  for (std::size_t y = 0; y < board_.size(); ++y) {
    // Check if the row has no zeros (if it is full)
    bool isFull = std::none_of(board_[y].begin(), board_[y].end(),
                               [](int i) { return i == 0; });

    if (isFull) {
      // Remove the full line
      board_.erase(board_.begin() + y);
      // Insert a new empty line at the top of the board
      board_.insert(board_.begin(), std::vector<int>(10, 0));

      // Add +1 to the tetrisCount_/kCount;
      tetrisCount_++;
      kCount++;
    }
  }

  setScore(kCount);
}

// ____________________________________________________________________________

bool Game::checkCollision(int dx, int dy, int rotation) {
  // Create a const ref with the given rotation
  const std::vector<std::vector<int>> &shape =
      currentTetromino_.getShape(rotation);

  // Add current coordinates to move direction to get future coordinates
  int newX = tetrominoX_ + dx;
  int newY = tetrominoY_ + dy;

  for (std::size_t y = 0; y < shape.size(); ++y) {
    for (std::size_t x = 0; x < shape[y].size(); ++x) {
      // If it's a ghost piece return false
      if (shape[y][x] > 8) {
        return false;
      } else if (shape[y][x] != 0) {
        std::size_t boardX = newX + x;
        std::size_t boardY = newY + y;

        // Check if out of bounds on the left side
        if (boardX < 0) {
          return true;
        }

        // Check if out of bounds on the right side with offset
        if (boardX >= board_[0].size()) {
          return true;
        }

        // Check if out of bounds on the bottom
        if (boardY >= board_.size()) {
          return true;
        }

        // Check if collides with existing blocks
        if (board_[boardY][boardX] != 0) {
          return true;
        }
      }
    }
  }
  return false;
}

// ____________________________________________________________________________

void Game::placeTetromino() {
  const std::vector<std::vector<int>> &shape = currentTetromino_.getShape();
  for (std::size_t y = 0; y < shape.size(); ++y) {
    for (std::size_t x = 0; x < shape[y].size(); ++x) {
      if (shape[y][x] != 0) {
        // Add current shape to board_
        board_[tetrominoY_ + y][tetrominoX_ + x] = shape[y][x];
      }
    }
  }
}

// ____________________________________________________________________________

void Game::spawnTetromino() {
  // Set random seed by getting the current ms;
  srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());

  // Assign nextTetrmino type to the current one.
  currentTetromino_.reset(nextTetromino_.getType());

  // Assign a new tetromino to nextTetromino.
  nextTetromino_.reset(static_cast<TetrominoType>(rand() % 7));

  // Assure that that the same type won't appear after another
  if (nextTetromino_.getType() == currentTetromino_.getType()) {
    nextTetromino_.reset(static_cast<TetrominoType>(rand() % 7));
  }

  tetrominoX_ = 4; // Starting x position
  tetrominoY_ = 0; // Starting y position
}

// ____________________________________________________________________________

void Game::setGhostPiece(TerminalManager &terminalManager) {
  int ghostY = tetrominoY_;

  // Find the lowest position where the current Tetromino can be placed without
  // collision
  while (!checkCollision(0, ghostY - tetrominoY_ + 1, 0)) {
    ghostY++;
  }

  // Draw the ghost Tetromino at the calculated position
  drawGhostPiece(terminalManager, ghostY);
}

// ____________________________________________________________________________

void Game::checkLevel() {
  if (currentLevel_ < 0) {
    // In order to test without moving pieces choose a level < 0;
    // One could also call it practice mode! It's a feature...
    mdTetromino_ = INT32_MAX;
  } else if (currentLevel_ >= 29) {
    mdTetromino_ = 1;
  } else if (currentLevel_ >= 19) {
    mdTetromino_ = 2;
  } else if (currentLevel_ >= 16) {
    mdTetromino_ = 3;
  } else if (currentLevel_ >= 13) {
    mdTetromino_ = 4;
  } else if (currentLevel_ >= 10) {
    mdTetromino_ = 5;
  } else if (currentLevel_ == 9) {
    mdTetromino_ = 6;
  } else if (currentLevel_ == 8) {
    mdTetromino_ = 8;
  } else if (currentLevel_ == 7) {
    mdTetromino_ = 13;
  } else if (currentLevel_ == 6) {
    mdTetromino_ = 18;
  } else if (currentLevel_ == 5) {
    mdTetromino_ = 23;
  } else if (currentLevel_ == 4) {
    mdTetromino_ = 28;
  } else if (currentLevel_ == 3) {
    mdTetromino_ = 33;
  } else if (currentLevel_ == 2) {
    mdTetromino_ = 38;
  } else if (currentLevel_ == 1) {
    mdTetromino_ = 43;
  } else {
    mdTetromino_ = 48;
  }
}

// ____________________________________________________________________________

void Game::increaseLevel() {
  if (tetrisCount_ >= 10) {
    currentLevel_++;
    tetrisCount_ -= 10; // Reset the count after increasing the level
  }
  checkLevel();
}

// ____________________________________________________________________________

void Game::setScore(int k) {
  // 40 * (i+1) für k = 1, 100 * (i+1) für k = 2,
  // 300 * (i+1) für k = 3, 1200* (i+1) für k = 4
  if (k == 0) {
    return;
  } else if (k == 1) {
    score_ += 40 * (currentLevel_ + 1);
  } else if (k == 2) {
    score_ += 100 * (currentLevel_ + 1);
  } else if (k == 3) {
    score_ += 300 * (currentLevel_ + 1);
  } else {
    score_ += 1200 * (currentLevel_ + 1);
  }
}

// ____________________________________________________________________________

bool Game::checkTopOut() {
  for (size_t i = 0; i < board_[0].size(); i++) {
    if (board_[0][i] != 0) {
      return true;
    }
  }
  return false;
}

// ____________________________________________________________________________

void Game::moveLeft() {
  if (!checkCollision(-1, 0, 0)) {
    tetrominoX_--;
  }
}

// ____________________________________________________________________________

void Game::moveRight() {
  if (!checkCollision(1, 0, 0)) {
    tetrominoX_++;
  }
}

// ____________________________________________________________________________

void Game::hardDrop() {
  for (int y = tetrominoY_; y < 20; y++) {
    if (checkCollision(0, 1, 0)) {
      // Place the Tetromino one row above the collision point
      placeTetromino();
      spawnTetromino();
      clearFullLines();
      return;
    }
    // Move the Tetromino down by one row
    tetrominoY_++;
  }
  // Place the Tetromino if it reaches the bottom without collision
  placeTetromino();
  spawnTetromino();
  clearFullLines();
}

// ____________________________________________________________________________

void Game::rotate(int rotation) {
  if (!checkCollision(0, 0, rotation)) {
    currentTetromino_.rotate(rotation);
  }
}

// ____________________________________________________________________________

void Game::togglePause() { paused_ = !paused_; }
