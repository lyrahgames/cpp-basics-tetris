#include <tetris/game.hpp>

namespace tetris {

game::game() { restart(); }

void game::move_right() {
  const auto tmp = ::tetris::move_right(current);
  if (!is_colliding(tmp, field)) current = tmp;
  // last = last_position(current, field);
}

void game::move_left() {
  const auto tmp = ::tetris::move_left(current);
  if (!is_colliding(tmp, field)) current = tmp;
  // last = last_position(current, field);
}

void game::rotate() {
  auto tmp = ::tetris::rotate(current);
  if (!is_colliding(tmp, field)) {
    current = tmp;
    return;
  }

  // wall kicks
  if (current.size == 2) return;
  if (current.size == 3) {
    switch (current.rotation) {
      case 0:
        tmp.offset[1] += -1;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[0] += 1;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[0] += -3;
        tmp.offset[1] += 1;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[1] += -1;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        break;
      case 1:
        tmp.offset[1] += 1;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[0] += -1;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[0] += 3;
        tmp.offset[1] += -1;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[1] += 1;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        break;
      case 2:
        tmp.offset[1] += 1;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[0] += 1;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[0] += -3;
        tmp.offset[1] += -1;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[1] += 1;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        break;
      case 3:
        tmp.offset[1] += -1;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[0] += -1;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[0] += 3;
        tmp.offset[1] += 1;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[1] += -1;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        break;
    }
  } else if (current.size == 4) {
    switch (current.rotation) {
      case 0:
        tmp.offset[1] += -2;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[1] += 3;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[0] += -1;
        tmp.offset[1] += -3;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[0] += 3;
        tmp.offset[1] += 3;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        break;
      case 1:
        tmp.offset[1] += -1;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[1] += 1;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[0] += 2;
        tmp.offset[1] += -3;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[0] -= 3;
        tmp.offset[1] += 3;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        break;
      case 2:
        tmp.offset[1] += 2;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[0] += -3;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[0] += 1;
        tmp.offset[1] += 1;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[0] += -3;
        tmp.offset[1] += -3;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        break;
      case 3:
        tmp.offset[1] += 1;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[1] += -3;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[0] += -2;
        tmp.offset[1] += 3;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        tmp.offset[0] += -3;
        tmp.offset[1] += 3;
        if (!is_colliding(tmp, field)) {
          current = tmp;
          return;
        }
        break;
    }
  }
}

void game::new_tetromino() {
  current = next;
  next = tetromino{static_cast<label>(dist(rng))};
  next.offset[0] = (field.visible_rows - 1) + (3 - next.size);
  next.offset[1] = (playfield::cols - 1) / 2 - (next.size - 1) / 2;
  // last = last_position(current, field);
}

void game::new_round() {
  transfer(current, field);
  lines_cleared = lines_cleared + check_full_rows(field);
  level = 1 + lines_cleared / 5;
  std::cout << "Lines Cleared:\t" << lines_cleared << "\n"
            << "Level:\t" << level << "\n"
            << "Gravity:\t" << time_steps[level - 1] << "\n\n";
  new_tetromino();
  if (is_colliding(current, field)) game_over = true;
}

void game::advance() {
  if (game_over) return;
  const auto falling_piece = ::tetris::move_down(current);
  if (is_colliding(falling_piece, field)) {
    new_round();
  } else {
    current = falling_piece;
  }
  // last = last_position(current, field);
}

void game::advance_to_next_round() {
  if (game_over) return;
  current = last_position(current, field);
  new_round();
}

void game::restart() {
  game_over = true;  // false;
  lines_cleared = 0;
  level = 1;
  new_tetromino();
  new_tetromino();
  field.clear();
}

void game::update() {
  last = last_position(current, field);

  const auto new_time = std::chrono::high_resolution_clock::now();
  const auto duration = std::chrono::duration<float>(new_time - time).count();
  if (duration < time_steps[level - 1]) return;

  time = new_time;
  advance();
}

}  // namespace tetris