#pragma once
#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <random>
#include <tetris/playfield.hpp>
#include <tetris/seven_bag_distribution.hpp>
#include <tetris/tetromino.hpp>

namespace tetris {

struct game {
  game();
  void move_right();
  void move_left();
  void rotate();
  void new_tetromino();
  void new_round();
  void advance();
  void advance_to_next_round();
  void restart();
  void update();

  playfield field;
  tetromino current;
  tetromino last;
  tetromino next;
  // static constexpr float time_step = 0.5;
  static constexpr float time_steps[] = {
      1.00000, 0.79300, 0.61780, 0.47273, 0.35520, 0.26200, 0.18968,
      0.13473, 0.09388, 0.06415, 0.04298, 0.02822, 0.01815, 0.01144,
      0.00706, 0.00426, 0.00252, 0.00146, 0.00082, 0.00046};
  std::mt19937 rng{std::random_device{}()};
  seven_bag_distribution dist{};
  bool game_over = false;
  int lines_cleared = 0;
  decltype(std::chrono::high_resolution_clock::now()) time =
      std::chrono::high_resolution_clock::now();
  int level = 1;
};

}  // namespace tetris