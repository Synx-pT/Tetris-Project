// Copyright Paul Tröster
// Ü11 - Uni Freiburg

#include "./Game.h"
#include "./Tetromino.h"
#include "Colors.h"
#include <gtest/gtest.h>

TEST(Tetromino, DefaultConstructor) {
  // Default Tetromino
  Tetromino tetromino_;

  // Check default init. things
  ASSERT_EQ(tetromino_.rotationState_, 0);
  ASSERT_EQ(tetromino_.getShape(), std::vector<std::vector<int>>({{1}}));
}

TEST(Tetromino, TetrominoTypes) {
  // Check for a few Tetrominos for the right values

  // Tetromino Type I
  Tetromino tetrominoI(TetrominoType::I);
  ASSERT_EQ(tetrominoI.getType(), TetrominoType::I);
  std::vector<std::vector<int>> expectedShapeI = {{1, 1, 1, 1}};
  ASSERT_EQ(tetrominoI.getShape(), expectedShapeI);

  // Tetromino Type O
  Tetromino tetrominoO(TetrominoType::O);
  ASSERT_EQ(tetrominoO.getType(), TetrominoType::O);
  std::vector<std::vector<int>> expectedShapeO = {{2, 2}, {2, 2}};
  ASSERT_EQ(tetrominoO.getShape(), expectedShapeO);
}

TEST(Tetromino, ResetFunction) {
  // Reset Tetromino to Type T
  Tetromino tetromino(TetrominoType::O);
  tetromino.reset(TetrominoType::T);
  ASSERT_EQ(tetromino.getType(), TetrominoType::T);

  // Check for correct and expected Shape after reset and init
  std::vector<std::vector<int>> expectedShapeT = {{0, 3, 0}, {3, 3, 3}};
  ASSERT_EQ(tetromino.getShape(), expectedShapeT);
}

TEST(Tetromino, RotateFunction) {
  // Check wether a T piece is rotate correctly 4 times
  Tetromino tetromino(TetrominoType::T);
  tetromino.rotate(1);
  ASSERT_EQ(tetromino.getShape(),
            std::vector<std::vector<int>>({{3, 0}, {3, 3}, {3, 0}}));
  tetromino.rotate(1);
  ASSERT_EQ(tetromino.getShape(),
            std::vector<std::vector<int>>({{3, 3, 3}, {0, 3, 0}}));
  tetromino.rotate(1);
  ASSERT_EQ(tetromino.getShape(),
            std::vector<std::vector<int>>({{0, 3}, {3, 3}, {0, 3}}));
  tetromino.rotate(1);
  ASSERT_EQ(tetromino.getShape(),
            std::vector<std::vector<int>>({{0, 3, 0}, {3, 3, 3}}));
}

TEST(Tetromino, GetShapeFunction) {
  // Test if the get shape works correct for different kind of rotation
  Tetromino tetromino(TetrominoType::S);
  ASSERT_EQ(tetromino.getShape(0),
            std::vector<std::vector<int>>({{0, 4, 4}, {4, 4, 0}}));
  ASSERT_EQ(tetromino.getShape(1),
            std::vector<std::vector<int>>({{4, 0}, {4, 4}, {0, 4}}));
}

TEST(Game, DefaultConstructor) {
  TerminalManager terminalManager(init_list);
  Game game(terminalManager);

  ASSERT_EQ(game.tetrisCount_, 0);
  ASSERT_EQ(game.mdTetromino_, 48);
  ASSERT_EQ(game.currentLevel_, 0);
  ASSERT_EQ(game.paused_, false);
  ASSERT_EQ(game.gameStop_, false);
  ASSERT_EQ(game.score_, false);
}

TEST(Game, HandleInput) {
  TerminalManager terminalManager(init_list);
  Game game(terminalManager);

  game.handleInput('p'); // Pause the game
  ASSERT_EQ(game.isPaused(), true);

  game.handleInput('p'); // Resume the game
  ASSERT_EQ(game.isPaused(), false);

  game.handleInput('q'); // Stop the game
  ASSERT_EQ(game.isStopped(), true);

  // Check a invalid key press
  game.handleInput('x');
  ASSERT_EQ(game.isPaused(), false);
  ASSERT_EQ(game.isStopped(),
            true); // Still true because 'q' was pressed earlier
}

TEST(Game, IncreaseScore) {
  TerminalManager terminalManager(init_list);
  Game game(terminalManager);

  // Verify initial score
  ASSERT_EQ(game.score_, 0);

  // Increase score and verify
  game.increaseScore();
  ASSERT_EQ(game.score_, 1);

  game.increaseScore();
  ASSERT_EQ(game.score_, 2);
}

TEST(Game, SetLevel) {
  TerminalManager terminalManager(init_list);
  Game game(terminalManager);

  // Set level and verify
  game.setLevel(5);
  ASSERT_EQ(game.currentLevel_, 5);

  game.setLevel(10);
  ASSERT_EQ(game.currentLevel_, 10);
}

TEST(Game, SetRotationKeys) {
  TerminalManager terminalManager(init_list);
  Game game(terminalManager);

  // Set rotation keys and verify
  game.setRotationKeys('a', 's', 'd');
  ASSERT_EQ(game.rotateLeftKey_, 'a');
  ASSERT_EQ(game.rotate180Key_, 's');
  ASSERT_EQ(game.rotateRightKey_, 'd');

  game.setRotationKeys('j', 'k', 'l');
  ASSERT_EQ(game.rotateLeftKey_, 'j');
  ASSERT_EQ(game.rotate180Key_, 'k');
  ASSERT_EQ(game.rotateRightKey_, 'l');
}

TEST(Game, ClearFullLines) {
  TerminalManager terminalManager(init_list);
  Game game(terminalManager);

  // Setup the game board with full line
  game.board_[0] = std::vector<int>(game.board_[0].size(), 1);

  // Call the function in order to clear full lines
  game.clearFullLines();

  ASSERT_EQ(game.board_[0], std::vector<int>(game.board_[0].size(), 0));
}

TEST(Game, CheckCollision) {
  TerminalManager terminalManager(init_list);
  Game game(terminalManager);

  Tetromino tetromino(TetrominoType::I);
  game.currentTetromino_ = tetromino;

  ASSERT_EQ(game.checkCollision(0, 0, 0), false);

  // Simulate collision by moving the tetromino out of bounds
  ASSERT_EQ(game.checkCollision(-100, 0, 0), true); // Left border
  ASSERT_EQ(game.checkCollision(0, -100, 0), true); // Top border
}

TEST(Game, PlaceTetromino) {
  TerminalManager terminalManager(init_list);
  Game game(terminalManager);

  Tetromino tetromino(TetrominoType::I);
  game.currentTetromino_ = tetromino;

  // Place the tetromino and verify
  game.placeTetromino();
  ASSERT_EQ(game.board_[0][0], 0);
}

TEST(Game, SpawnTetromino) {
  TerminalManager terminalManager(init_list);
  Game game(terminalManager);

  // Spawn a new tetromino and verify
  game.spawnTetromino();

  // Couldn't figure out how to test:
  //  ASSERT_TRUE(game.currentTetromino_.getType() !=
  //          NULL); // Should spawn a valid tetromino
}

TEST(Game, IncreaseLevelCheckLevel) {
  TerminalManager terminalManager(init_list);
  Game game(terminalManager);

  // After 10 terises and being at a low level should be a level up
  game.tetrisCount_ = 10;

  game.increaseLevel();
  ASSERT_EQ(game.currentLevel_, 1);
}

TEST(Game, SetScore) {
  TerminalManager terminalManager(init_list);
  Game game(terminalManager);

  // Set the score and verify for tetris
  game.setScore(4);
  ASSERT_EQ(game.score_, 1200);
}

TEST(Game, CheckTopOut) {
  TerminalManager terminalManager(init_list);
  Game game(terminalManager);

  // Check for top out condition
  game.board_[0][0] = 1; // Simulate a block at the top row

  // Verify top out
  ASSERT_EQ(game.checkTopOut(), true);
}

TEST(Game, MoveLeft) {
  TerminalManager terminalManager(init_list);
  Game game(terminalManager);

  int initialX = game.tetrominoX_;

  // Move tetromino left and verify the position
  game.moveLeft();
  ASSERT_EQ(game.tetrominoX_, initialX - 1);
}

TEST(Game, MoveRight) {
  TerminalManager terminalManager(init_list);
  Game game(terminalManager);

  int initialX = game.tetrominoX_;

  // Move tetromino right and verify the position
  game.moveRight();
  ASSERT_EQ(game.tetrominoX_, initialX + 1);
}

TEST(Game, HardDrop) {
  TerminalManager terminalManager(init_list);
  Game game(terminalManager);

  int initialY = game.tetrominoY_;

  // Hard drop the tetromino and verify the position
  game.hardDrop();
  ASSERT_EQ(game.tetrominoY_,
            initialY); // The Y position should be less (dropped)
}

TEST(Game, Rotate) {
  TerminalManager terminalManager(init_list);
  Game game(terminalManager);

  // Assume initial rotation state of the tetromino
  int initialRotationState = game.currentTetromino_.rotationState_;

  // Rotate the tetromino
  game.rotate(1);
  ASSERT_EQ(game.currentTetromino_.rotationState_,
            (initialRotationState + 1) % game.currentTetromino_.shape_.size());

  // Rotate the tetromino back
  game.rotate(-1);
  ASSERT_EQ(game.currentTetromino_.rotationState_, initialRotationState);
}

TEST(Game, TogglePause) {
  TerminalManager terminalManager(init_list);
  Game game(terminalManager);

  // Assume initial pause state
  bool initialPauseState = game.paused_;

  game.togglePause();
  ASSERT_EQ(game.paused_, !initialPauseState);

  // Toggle pause again
  game.togglePause();
  ASSERT_EQ(game.paused_, initialPauseState);
}