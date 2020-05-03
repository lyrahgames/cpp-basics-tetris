#pragma once
#include <array>
// #include <chrono>
#include <tetris/scoring_system.hpp>
#include <tetris/tetromino.hpp>
#include <thread>

namespace tetris {

struct playfield {
  label& operator()(int i, int j) noexcept { return data[i * cols + j]; }
  const label& operator()(int i, int j) const noexcept {
    return data[i * cols + j];
  }
  void clear() noexcept { data = decltype(data){}; }

  static constexpr size_t cols = 10;
  static constexpr size_t rows = 40;
  static constexpr size_t visible_rows = 20;
  std::array<label, cols * rows> data{};
};

inline bool row_is_full(const playfield& field, int i) noexcept {
  for (int j = 0; j < field.cols; ++j)
    if (field(i, j) == label::empty) return false;
  return true;
}

inline int check_full_rows(playfield& field) noexcept {
  int result = 0;
  for (int i = 0; i < field.visible_rows; ++i) {
    while (row_is_full(field, i)) {
      ++result;
      for (int p = i; p < field.visible_rows; ++p) {
        for (int q = 0; q < field.cols; ++q) {
          field(p, q) = field(p + 1, q);
        }
      }
    }
  }
  return result;
}

inline bool is_colliding(const tetromino& t, const playfield& field) noexcept {
  for (const auto& e : t.shape) {
    const auto p = t.offset[0] + e[0];
    const auto q = t.offset[1] + e[1];
    if (q < 0 || q >= field.cols || p < 0 ||
        field(p, q) != label::empty)  //|| p >= field.visible_rows)
      return true;
  }
  return false;
}

inline void transfer(const tetromino& t, playfield& field) noexcept {
  for (const auto e : t.shape)
    field(t.offset[0] + e[0], t.offset[1] + e[1]) = t.type;
}

inline tetromino last_position(tetromino t, const playfield& field) noexcept {
  while (!is_colliding(t, field)) t.offset[0] -= 1;
  t.offset[0] += 1;
  return t;
}

}  // namespace tetris