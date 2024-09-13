// Copyright Paul Tröster
// Ü11 - Uni Freiburg

#pragma once
#include "TerminalManager.h"
#include "Tetromino.h"
#include <algorithm>
#include <gtest/gtest.h>
#include <vector>

// Handles Tetris Logic
class Game {
public:
  // Initialize Game
  Game(TerminalManager &terminalManager);

  // Update/refresh screen
  void update(TerminalManager &terminalManager);

  // Drawing ------------------------------------

  // Draw tetrminos
  void drawTetromino(TerminalManager &terminalManager);

  // Draw ghost tetrominos
  void drawGhostPiece(TerminalManager &terminalManager, int ghostY);

  // Draw the info panel right next to the playing board
  // The info panel includes:
  // Next piece, level, score
  void drawInfoPanel(TerminalManager &terminalManager);

  // Draw the border
  void drawBorder(TerminalManager &terminalManager);

  // Draw the board
  void drawBoard(TerminalManager &terminalManager);

  // Clean the inside of the border and next piece
  void clean(TerminalManager &terminalManager);

  // Input --------------------------------------

  // Handle input
  void handleInput(char input);

  // Tetromino down
  // Public for main
  void moveDown();

  // Getters ------------------------------------

  // Check if game is paused/stopped
  bool isPaused() const { return paused_; };
  bool isStopped() const { return gameStop_; };

  // Get current mdTetromino int
  int mdTetromino() const { return mdTetromino_; };

  // --------------------------------------------

  // Increase score by 1
  void increaseScore() { score_++; };

  // Set level/keys
  void setLevel(int n) { currentLevel_ = n; };

  void setRotationKeys(char rotateLeft, char rotate180, char rotateRight) {
    rotateLeftKey_ = rotateLeft;
    rotate180Key_ = rotate180;
    rotateRightKey_ = rotateRight;
  }

private:
  // Helper Functions - Update Game -------------

  // Check and clear if a line is full
  void clearFullLines();

  // Check collision for tetrominos with border/tetrominos
  bool checkCollision(int dx, int dy, int rotation);

  // Place a tetromino in game board vector
  void placeTetromino();

  // Spawn a tetromino
  void spawnTetromino();

  // Helper function to set for drawGhostPiece
  void setGhostPiece(TerminalManager &terminalManager);

  // Check if the speed should increase according to the level
  void checkLevel();

  // Increase level if 10 lines are cleared
  void increaseLevel();

  // Calculate game score;
  void setScore(int k);

  // Check top out
  bool checkTopOut();

  // Helper Functions - Handle input ------------

  // Tetromino left
  void moveLeft();

  // Tetromino right
  void moveRight();

  // Tetromino hard drop
  void hardDrop();

  // Rotate tetromino
  void rotate(int rotation);

  // Toggle Pause of Game State
  void togglePause();

  // Game board ---------------------------------

  // Representing the game board
  std::vector<std::vector<int>> board_;

  // Border Size to shift the game logic
  static const int borderSize_ = 1;

  // Border color
  const int borderColor_ = 8;

  // Tetromino ----------------------------------

  // Current/Next tetromino which is used
  Tetromino currentTetromino_;
  Tetromino nextTetromino_;

  // Current position of tetromino
  int tetrominoX_, tetrominoY_;

  // Level up -----------------------------------

  // Amount of line clears
  int tetrisCount_;

  // Amount of frames when tetromino should move down
  // Level 0 speed = 48ms
  int mdTetromino_;

  // Current level.
  int currentLevel_;

  // --------------------------------------------

  // Default color to clean the screen
  const int cleanColor_ = 0;

  // Paused bool
  bool paused_;

  // Is game stopped
  bool gameStop_;

  // Game score
  int score_;

  // Rotation keys
  char rotateLeftKey_;
  char rotate180Key_;
  char rotateRightKey_;

  FRIEND_TEST(Game, DefaultConstructor);
  FRIEND_TEST(Game, IncreaseScore);
  FRIEND_TEST(Game, SetLevel);
  FRIEND_TEST(Game, SetRotationKeys);
  FRIEND_TEST(Game, ClearFullLines);
  FRIEND_TEST(Game, CheckCollision);
  FRIEND_TEST(Game, PlaceTetromino);
  FRIEND_TEST(Game, SpawnTetromino);
  FRIEND_TEST(Game, SetGhostPiece);
  FRIEND_TEST(Game, IncreaseLevelCheckLevel);
  FRIEND_TEST(Game, SetScore);
  FRIEND_TEST(Game, CheckTopOut);
  FRIEND_TEST(Game, MoveLeft);
  FRIEND_TEST(Game, MoveRight);
  FRIEND_TEST(Game, HardDrop);
  FRIEND_TEST(Game, Rotate);
  FRIEND_TEST(Game, TogglePause);
};
