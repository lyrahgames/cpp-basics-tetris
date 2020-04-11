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
  void move_right() {
    const auto tmp = ::tetris::move_right(current);
    if (!is_colliding(tmp, field)) current = tmp;
    last = last_position(current, field);
  }

  void move_left() {
    const auto tmp = ::tetris::move_left(current);
    if (!is_colliding(tmp, field)) current = tmp;
    last = last_position(current, field);
  }

  void rotate() {
    const auto tmp = ::tetris::rotate(current);
    if (!is_colliding(tmp, field)) current = tmp;
    last = last_position(current, field);
  }

  void new_tetromino() {
    current = tetromino{static_cast<label>(dist(rng))};
    current.offset[1] = (playfield::cols - 1) / 2 - (current.size - 1) / 2;
    last = last_position(current, field);
  }

  void new_round() {
    transfer(current, field);
    check_full_rows(field);
    new_tetromino();
    if (is_colliding(current, field)) game_over = true;
  }

  void advance() {
    if (game_over) return;

    const auto falling_piece = ::tetris::move_down(current);
    if (is_colliding(falling_piece, field)) {
      new_round();
    } else {
      current = falling_piece;
    }
    last = last_position(current, field);
  }

  void advance_to_next_round() {
    if (game_over) return;
    current = last_position(current, field);
    new_round();
  }

  void restart() {
    game_over = false;
    new_tetromino();
    field.clear();
  }

  game() { restart(); }

  playfield field;
  tetromino current;
  tetromino last;
  static constexpr float time_step = 0.5;
  std::mt19937 rng{std::random_device{}()};
  seven_bag_distribution dist{};
  bool game_over = false;
};

}  // namespace tetris