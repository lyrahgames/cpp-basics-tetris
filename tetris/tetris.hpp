#pragma once
#include <array>
#include <chrono>
#include <random>

namespace tetris {

struct tetris {
  using data_type = char;
  enum class tetrimino : data_type { EMPTY = 0, I, O, T, S, Z, J, L };

  tetris() { create_tetrimino(); }

  void create_tetrimino() {
    std::uniform_int_distribution<data_type> dist{1, 7};
    // for (auto& e : playfield_) e = static_cast<tetrimino>(dist(rng));
    current = static_cast<tetrimino>(dist(rng));
    // current = tetrimino::I;
    size = 3;
    offset = decltype(offset){};

    switch (current) {
      case tetris::tetrimino::I:
        current_position =
            decltype(current_position){{{0, 1}, {1, 1}, {2, 1}, {3, 1}}};
        size = 4;
        break;
      case tetris::tetrimino::O:
        current_position =
            decltype(current_position){{{0, 0}, {0, 1}, {1, 0}, {1, 1}}};
        size = 2;
        break;
      case tetris::tetrimino::T:
        current_position =
            decltype(current_position){{{0, 1}, {1, 1}, {2, 1}, {1, 2}}};
        break;
      case tetris::tetrimino::S:
        current_position =
            decltype(current_position){{{1, 0}, {2, 0}, {0, 1}, {1, 1}}};
        break;
      case tetris::tetrimino::Z:
        current_position =
            decltype(current_position){{{0, 0}, {1, 0}, {1, 1}, {2, 1}}};
        break;
      case tetris::tetrimino::J:
        current_position =
            decltype(current_position){{{1, 0}, {1, 1}, {1, 2}, {0, 2}}};
        break;
      case tetris::tetrimino::L:
        current_position =
            decltype(current_position){{{1, 0}, {1, 1}, {1, 2}, {2, 2}}};
        break;
    }
  }

  bool is_colliding() {
    for (auto& e : current_position) {
      if (offset[0] + e[0] < 0 || offset[0] + e[0] >= cols ||
          offset[1] + e[1] < 0 || offset[1] + e[1] >= rows ||
          playfield(offset[1] + e[1], offset[0] + e[0]) != tetrimino::EMPTY) {
        return true;
      }
    }
    return false;
  }

  // bool is_colliding(decltype(current_position) t){
  //   for (auto& e : t) {
  //     if (e[1] + 1 >= rows || playfield(e[1] + 1, e[0]) != tetrimino::EMPTY)
  //     {
  //       return true;
  //     }
  //   }
  //   return false;
  // }

  void restart() {
    game_over = false;
    create_tetrimino();
    for (auto& t : playfield_) t = tetrimino::EMPTY;
  }

  void move_right() {
    offset[0] += 1;
    if (is_colliding()) {
      offset[0] -= 1;
    }
  }

  void move_left() {
    offset[0] -= 1;
    if (is_colliding()) {
      offset[0] += 1;
    }
  }

  void rotate() {
    for (auto& e : current_position) {
      const auto tmp = e[0];
      e[0] = -e[1];
      e[1] = tmp;

      e[0] += (size - 1);
      e[1] += 0;
    }
    // if (is_colliding()) {
    //   for (auto& e : current_position) {
    //     e[0] -= offset[0];
    //     e[1] -= offset[1];

    //     const auto tmp = e[0];
    //     e[0] = e[1];
    //     e[1] = -tmp;

    //     e[0] += offset[0];
    //     e[1] += offset[1];
    //   }
    // }
  }

  void check_full_rows() {}

  void advance() {
    if (game_over) return;
    // for (auto& e : current_position) {
    //   e[1] += 1;
    // }
    offset[1] += 1;
    bool collision = is_colliding();
    if (collision) {
      // for (auto& e : current_position) {
      //   e[1] -= 1;
      // }
      offset[1] -= 1;
      for (const auto e : current_position)
        playfield(offset[1] + e[1], offset[0] + e[0]) = current;
      create_tetrimino();
      if (is_colliding()) game_over = true;
    }
  }

  tetrimino& playfield(int i, int j) noexcept {
    return playfield_[i * cols + j];
  }
  const tetrimino& playfield(int i, int j) const noexcept {
    return playfield_[i * cols + j];
  }

  static constexpr size_t cols = 10;
  static constexpr size_t rows = 22;
  static constexpr float time_step = 0.2;
  std::array<tetrimino, cols * rows> playfield_{};
  std::array<std::array<int, 2>, 4> current_position{};
  tetrimino current{};
  int size{};
  std::array<int, 2> offset{};
  std::mt19937 rng{std::random_device{}()};
  bool game_over = false;
};

}  // namespace tetris