#pragma once
#include <array>

namespace tetris {

enum class label : char { empty = 0, I, O, T, S, Z, J, L };

struct tetromino {
  using element_type = std::array<int, 2>;

  tetromino() = default;
  explicit tetromino(label l) : type(l) {
    switch (l) {
      case label::empty:
      case label::I:
        shape = decltype(shape){{{2, 0}, {2, 1}, {2, 2}, {2, 3}}};
        size = 4;
        break;
      case label::O:
        shape = decltype(shape){{{0, 0}, {0, 1}, {1, 0}, {1, 1}}};
        size = 2;
        break;
      case label::T:
        shape = decltype(shape){{{1, 0}, {1, 1}, {1, 2}, {2, 1}}};
        break;
      case label::S:
        shape = decltype(shape){{{1, 0}, {1, 1}, {2, 1}, {2, 2}}};
        break;
      case label::Z:
        shape = decltype(shape){{{2, 0}, {2, 1}, {1, 1}, {1, 2}}};
        break;
      case label::J:
        shape = decltype(shape){{{2, 0}, {1, 0}, {1, 1}, {1, 2}}};
        break;
      case label::L:
        shape = decltype(shape){{{1, 0}, {1, 1}, {1, 2}, {2, 2}}};
        break;
    }
  }

  std::array<element_type, 4> shape{};
  element_type offset{};
  size_t size{3};
  label type;
  int rotation = 0;
};

inline tetromino rotate(tetromino t) noexcept {
  for (auto& e : t.shape) {
    const auto tmp = e[1];
    e[1] = e[0];
    e[0] = -tmp;

    e[0] += (t.size - 1);
  }
  t.rotation = (t.rotation + 1) & 0b0011;
  return t;
}

inline tetromino move_right(tetromino t) noexcept {
  t.offset[1] += 1;
  return t;
}

inline tetromino move_left(tetromino t) noexcept {
  t.offset[1] -= 1;
  return t;
}

inline tetromino move_down(tetromino t) noexcept {
  t.offset[0] -= 1;
  return t;
}

}  // namespace tetris