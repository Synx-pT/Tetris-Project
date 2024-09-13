// Copyright Paul Tröster
// Ü11 - Uni Freiburg

#pragma once
#include "TerminalManager.h"
#include <vector>

// Tetromino colors
const Color black(0, 0, 0);
const Color cyan(0, 1, 1);
const Color yellow(1, 1, 0);
const Color purple(0.5, 0, 0.5);
const Color green(0, 1, 0);
const Color red(1, 0, 0);
const Color blue(0, 0, 1);
const Color orange(1, 0.5, 0);
const Color white(1, 1, 1);

// Ghost colors
const Color ghost_cyan(0, 0.5, 0.5);
const Color ghost_yellow(0.5, 0.5, 0);
const Color ghost_purple(0.25, 0, 0.25);
const Color ghost_green(0, 0.5, 0);
const Color ghost_red(0.5, 0, 0);
const Color ghost_blue(0, 0, 0.5);
const Color ghost_orange(0.5, 0.25, 0);

// Vector of color pairs for terminal initialization
const std::vector<std::pair<Color, Color>> init_list = {
    {black, white},
    {cyan, cyan},
    {yellow, yellow},
    {purple, purple},
    {green, green},
    {red, red},
    {blue, blue},
    {orange, orange},
    {white, black},
    {ghost_cyan, ghost_cyan},
    {ghost_yellow, ghost_yellow},
    {ghost_purple, ghost_purple},
    {ghost_green, ghost_green},
    {ghost_red, ghost_red},
    {ghost_blue, ghost_blue},
    {ghost_orange, ghost_orange}};