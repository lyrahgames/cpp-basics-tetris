#include <tetris/game.hpp>

namespace tetris {

game::game() { restart(); }

void game::move_right() {
  const auto tmp = ::tetris::move_right(current);
  if (!is_colliding(tmp, field)) current = tmp;
  last = last_position(current, field);
}

void game::move_left() {
  const auto tmp = ::tetris::move_left(current);
  if (!is_colliding(tmp, field)) current = tmp;
  last = last_position(current, field);
}

void game::rotate() {
  const auto tmp = ::tetris::rotate(current);
  if (!is_colliding(tmp, field)) current = tmp;
  last = last_position(current, field);
}

void game::new_tetromino() {
  current = next;
  next = tetromino{static_cast<label>(dist(rng))};
  next.offset[0] = 20;
  next.offset[1] = (playfield::cols - 1) / 2 - (next.size - 1) / 2;
  last = last_position(current, field);
}

void game::new_round() {
  transfer(current, field);
  lines_cleared += check_full_rows(field);
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
  last = last_position(current, field);
}

void game::advance_to_next_round() {
  if (game_over) return;
  current = last_position(current, field);
  new_round();
}

void game::restart() {
  game_over = false;
  lines_cleared = 0;
  level = 1;
  new_tetromino();
  new_tetromino();
  field.clear();
}

void game::update() {
  const auto new_time = std::chrono::high_resolution_clock::now();
  const auto duration = std::chrono::duration<float>(new_time - time).count();
  if (duration < time_steps[level - 1]) return;
  time = new_time;
  advance();
}

}  // namespace tetris