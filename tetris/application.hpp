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
  size_t screen_width;
  size_t screen_height;
  sf::RenderWindow window;
  sf::Texture texture{};
  sf::Sprite sprite{};
  std::vector<uint8_t> pixels{};

 private:
  static constexpr size_t tetrimino_size = 18;
  static constexpr size_t border_size = 3;
  static constexpr size_t min_screen_width =
      playfield::cols * (tetrimino_size + border_size) + border_size;
  static constexpr size_t min_screen_height =
      playfield::rows * (tetrimino_size + border_size) + border_size;

  void draw_playfield();
  void draw_tetrimino();

  // tetris game{};
  ::tetris::game g;
};

}  // namespace tetris