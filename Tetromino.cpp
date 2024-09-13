// Copyright Paul Tröster
// Ü11 - Uni Freiburg

#include "Tetromino.h"

Tetromino::Tetromino() {
  // Default shape - a pixel
  shape_ = {{{1}}};
  rotationState_ = 0;
}

Tetromino::Tetromino(TetrominoType type) {
  assignShape(type);
  rotationState_ = 0;
}

// Reset the Tetromino with a new type
void Tetromino::reset(TetrominoType type) {
  assignShape(type);
  rotationState_ = 0;
}

void Tetromino::rotate(int rotation) {
  rotationState_ = (rotationState_ + rotation) % shape_.size();
}

// If 0 theres no pixel, all other numbers mean theres a pixel in the respective color
void Tetromino::assignShape(TetrominoType type) {
  type_ = type;

  switch (type) {
  case TetrominoType::I:
    shape_ = {{{1, 1, 1, 1}}, {{1}, {1}, {1}, {1}}};
    break;
  case TetrominoType::O:
    shape_ = {{{2, 2}, {2, 2}}};
    break;
  case TetrominoType::T:
    shape_ = {{{0, 3, 0}, {3, 3, 3}},
              {{3, 0}, {3, 3}, {3, 0}},
              {{3, 3, 3}, {0, 3, 0}},
              {{0, 3}, {3, 3}, {0, 3}}};
    break;
  case TetrominoType::S:
    shape_ = {{{0, 4, 4}, {4, 4, 0}}, {{4, 0}, {4, 4}, {0, 4}}};
    break;
  case TetrominoType::Z:
    shape_ = {{{5, 5, 0}, {0, 5, 5}}, {{0, 5}, {5, 5}, {5, 0}}};
    break;
  case TetrominoType::J:
    shape_ = {{{6, 0, 0}, {6, 6, 6}},
              {{6, 6}, {6, 0}, {6, 0}},
              {{6, 6, 6}, {0, 0, 6}},
              {{0, 6}, {0, 6}, {6, 6}}};
    break;
  case TetrominoType::L:
    shape_ = {{{0, 0, 7}, {7, 7, 7}},
              {{7, 0}, {7, 0}, {7, 7}},
              {{7, 7, 7}, {7, 0, 0}},
              {{7, 7}, {0, 7}, {0, 7}}};
    break;
  default:
    shape_ = {{{0}}};
    break;
  }
}
