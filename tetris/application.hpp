#pragma once
#include <SFML/Graphics.hpp>
#include <tetris/tetris.hpp>
#include <vector>

namespace tetris {

class application {
 public:
  application();
  ~application() = default;

  void execute();

 private:
  void process_events();
  void render();
  void resize();

 private:
  sf::RenderWindow window;
  sf::View view;
  float cell_size = 20.0f;
  float min_cell_size = 15.0f;
  float max_cell_size = 40.0f;
  float cell_outline_size = 0.05f;
  float ghost_cell_outline_size = 0.1f;
  float tetrion_border_size = 0.5f;
  float tetrion_outline_size = 0.1f;
  float outer_border_size = 10.0f;

 private:
  void draw_playfield();
  void draw_tetrimino();
  void draw_last();
  void draw_next();

  ::tetris::game g;
};

}  // namespace tetris