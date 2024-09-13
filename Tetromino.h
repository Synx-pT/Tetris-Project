// Copyright Paul Tröster
// Ü11 - Uni Freiburg

#pragma once

#include <gtest/gtest.h>
#include <vector>

// Different types of tetrominos
enum class TetrominoType { I, O, T, S, Z, J, L };

// A class for each tetromino to handle the state, shape and rotation
class Tetromino {
public:
  // Default constructor
  Tetromino();

  // Constructor to determine the type of the tetromino
  Tetromino(TetrominoType type);

  // Reset the Tetromino with a new type
  void reset(TetrominoType type);

  // Return the shape, according to the rotation
  std::vector<std::vector<int>> getShape(int rotation = 0) const {
    return shape_[(rotationState_ + rotation) % shape_.size()];
  };

  // Return the type.
  TetrominoType getType() const { return type_; }

  // Rotating logic.
  void rotate(int rotation);

private:
  // The shape stored as type
  TetrominoType type_;

  // The shape of the tetromino in a vector
  std::vector<std::vector<std::vector<int>>> shape_;

  // 0 : 0°, 1 : 90° right, 2 : 180°, 3 : 270°
  int rotationState_;

  // Helper function for constructor.
  void assignShape(TetrominoType type);

  FRIEND_TEST(Tetromino, DefaultConstructor);
  FRIEND_TEST(Tetromino, TetrominoTypes);
  FRIEND_TEST(Tetromino, ResetFunction);
  FRIEND_TEST(Tetromino, RotateFunction);
  FRIEND_TEST(Tetromino, GetShapeFunction);
  FRIEND_TEST(Tetromino, ComprehensiveRotationTest);
  FRIEND_TEST(Game, Rotate);
};