#include <chrono>
#include <tetris/application.hpp>

namespace tetris {

application::application()
    : screen_width{min_screen_width},
      screen_height{min_screen_height},
      window(sf::VideoMode(screen_width, screen_height), "Tetris") {
  window.setVerticalSyncEnabled(true);
  resize();
}

void application::execute() {
  auto time = std::chrono::system_clock::now();
  while (window.isOpen()) {
    const auto new_time = std::chrono::system_clock::now();
    if (std::chrono::duration<float>(new_time - time).count() >=
        game::time_step) {
      g.advance();
      time = new_time;
    }

    process_events();
    render();
  }
}

void application::process_events() {
  sf::Event event{};
  while (window.pollEvent(event)) {
    switch (event.type) {
      case sf::Event::Closed:
        window.close();
        break;

      case sf::Event::Resized:
        screen_width =
            std::max(static_cast<size_t>(event.size.width), min_screen_width);
        screen_height =
            std::max(static_cast<size_t>(event.size.height), min_screen_height);
        // window.setSize({screen_width, screen_height});
        resize();
        break;

      case sf::Event::KeyPressed:
        switch (event.key.code) {
          case sf::Keyboard::Escape:
            window.close();
            break;

          case sf::Keyboard::Space:
            g.advance_to_next_round();
            break;

          case sf::Keyboard::Down:
            g.advance();
            break;

          case sf::Keyboard::R:
            g.restart();
            break;

          case sf::Keyboard::Right:
            g.move_right();
            break;

          case sf::Keyboard::Left:
            g.move_left();
            break;

          case sf::Keyboard::Up:
            g.rotate();
            break;
        }
        break;
    }
  }
}

void application::render() {
  draw_playfield();
  draw_tetrimino();
  texture.update(pixels.data());
  window.clear();
  window.draw(sprite);
  window.display();
}

void application::resize() {
  pixels.resize(4 * screen_width * screen_height);
  window.setView(sf::View{sf::FloatRect{0, 0, static_cast<float>(screen_width),
                                        static_cast<float>(screen_height)}});
  texture.create(screen_width, screen_height);
  sprite.setTexture(texture, true);
}

void application::draw_playfield() {
  pixels.clear();

  for (int j = 0; j < playfield::rows; ++j) {
    for (int i = 0; i < playfield::cols; ++i) {
      uint8_t red = 0, green = 0, blue = 0;
      switch (g.field(j, i)) {
        case label::I:
          red = 0;
          green = 128;
          blue = 128;
          break;
        case label::O:
          red = 128;
          green = 128;
          blue = 0;
          break;
        case label::T:
          red = 128;
          green = 0;
          blue = 128;
          break;
        case label::S:
          red = 0;
          green = 128;
          blue = 0;
          break;
        case label::Z:
          red = 128;
          green = 0;
          blue = 0;
          break;
        case label::J:
          red = 128;
          green = 128;
          blue = 255;
          break;
        case label::L:
          red = 255;
          green = 128;
          blue = 0;
          break;
        case label::empty:
          red = 200;
          green = 200;
          blue = 200;
          break;
      }

      for (int q = 0; q < tetrimino_size; ++q) {
        for (int p = 0; p < tetrimino_size; ++p) {
          const int index =
              4 * (screen_width *
                       (q + j * (tetrimino_size + border_size) + border_size +
                        screen_height / 2 - min_screen_height / 2) +
                   (p + i * (tetrimino_size + border_size) + border_size +
                    screen_width / 2 - min_screen_width / 2));
          pixels[index + 0] = red;
          pixels[index + 1] = green;
          pixels[index + 2] = blue;
          pixels[index + 3] = 255;
        }
      }
    }
  }
}

void application::draw_tetrimino() {
  int red, green, blue;
  switch (g.current.type) {
    case label::I:
      red = 0;
      green = 128;
      blue = 255;
      break;
    case label::O:
      red = 255;
      green = 255;
      blue = 0;
      break;
    case label::T:
      red = 128;
      green = 0;
      blue = 128;
      break;
    case label::S:
      red = 0;
      green = 255;
      blue = 0;
      break;
    case label::Z:
      red = 255;
      green = 0;
      blue = 0;
      break;
    case label::J:
      red = 0;
      green = 0;
      blue = 255;
      break;
    case label::L:
      red = 255;
      green = 128;
      blue = 0;
      break;
    case label::empty:
      red = 200;
      green = 200;
      blue = 200;
      break;
  }

  for (const auto& e : g.current.shape) {
    for (int q = 0; q < tetrimino_size; ++q) {
      for (int p = 0; p < tetrimino_size; ++p) {
        const int index =
            4 *
            (screen_width *
                 (q +
                  (g.current.offset[0] + e[0]) *
                      (tetrimino_size + border_size) +
                  border_size + screen_height / 2 - min_screen_height / 2) +
             (p +
              (g.current.offset[1] + e[1]) * (tetrimino_size + border_size) +
              border_size + screen_width / 2 - min_screen_width / 2));
        pixels[index + 0] = red;
        pixels[index + 1] = green;
        pixels[index + 2] = blue;
        pixels[index + 3] = 255;
      }
    }
  }
}

}  // namespace tetris