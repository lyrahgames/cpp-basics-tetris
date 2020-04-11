#include <chrono>
#include <tetris/application.hpp>

namespace tetris {

namespace {

inline sf::Color label_color(label l) noexcept {
  switch (l) {
    case label::I:
      return sf::Color{50, 180, 255};
      break;
    case label::O:
      return sf::Color{220, 200, 30};
      break;
    case label::T:
      return sf::Color{200, 50, 200};
      break;
    case label::S:
      return sf::Color{50, 200, 50};
      break;
    case label::Z:
      return sf::Color{255, 50, 20};
      break;
    case label::J:
      return sf::Color{80, 80, 230};
      break;
    case label::L:
      return sf::Color{255, 100, 20};
      break;
    case label::empty:
      return sf::Color{50, 50, 50};
      break;
  }
  return sf::Color{};
}

}  // namespace

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
  for (auto& x : pixels) x = 0;
  draw_playfield();
  draw_next();
  draw_last();
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
  for (int j = 0; j < playfield::rows; ++j) {
    for (int i = 0; i < playfield::cols; ++i) {
      const auto c = label_color(g.field(j, i));

      for (int q = 0; q < tetrimino_size; ++q) {
        for (int p = 0; p < tetrimino_size; ++p) {
          const int index =
              4 * (screen_width * (q + j * (tetrimino_size + border_size) +
                                   border_size + outline_size +
                                   screen_height / 2 - min_screen_height / 2) +
                   (p + i * (tetrimino_size + border_size) + border_size +
                    outline_size + screen_width / 2 - min_screen_width / 2));
          pixels[index + 0] = c.r;
          pixels[index + 1] = c.g;
          pixels[index + 2] = c.b;
          pixels[index + 3] = c.a;
        }
      }
    }
  }
}

void application::draw_tetrimino() {
  const auto c = label_color(g.current.type);

  for (const auto& e : g.current.shape) {
    for (int q = 0; q < tetrimino_size; ++q) {
      for (int p = 0; p < tetrimino_size; ++p) {
        const int index =
            4 *
            (screen_width * (q +
                             (g.current.offset[0] + e[0]) *
                                 (tetrimino_size + border_size) +
                             border_size + outline_size + screen_height / 2 -
                             min_screen_height / 2) +
             (p +
              (g.current.offset[1] + e[1]) * (tetrimino_size + border_size) +
              border_size + outline_size + screen_width / 2 -
              min_screen_width / 2));
        pixels[index + 0] = c.r;
        pixels[index + 1] = c.g;
        pixels[index + 2] = c.b;
        pixels[index + 3] = c.a;
      }
    }
  }
}

void application::draw_next() {
  const auto c = label_color(g.next.type);

  for (const auto& e : g.next.shape) {
    for (int q = 0; q < tetrimino_size; ++q) {
      for (int p = 0; p < tetrimino_size; ++p) {
        const int index =
            4 * (screen_width * (q +
                                 (g.next.offset[0] + e[0]) *
                                     (tetrimino_size + border_size) +
                                 border_size + outline_size +
                                 screen_height / 2 - min_screen_height / 2) +
                 (p +
                  (playfield::cols + g.next.offset[1] + e[1]) *
                      (tetrimino_size + border_size) +
                  2 * border_size + outline_size + screen_width / 2 -
                  min_screen_width / 2));
        pixels[index + 0] = c.r;
        pixels[index + 1] = c.g;
        pixels[index + 2] = c.b;
        pixels[index + 3] = c.a;
      }
    }
  }
}

void application::draw_last() {
  const auto c = label_color(g.last.type);

  for (const auto& e : g.last.shape) {
    for (int q = 0; q < tetrimino_size; ++q) {
      for (int p = 0; p < border_size; ++p) {
        int index =
            4 *
            (screen_width *
                 (q +
                  (g.last.offset[0] + e[0]) * (tetrimino_size + border_size) +
                  border_size + outline_size + screen_height / 2 -
                  min_screen_height / 2) +
             (p + (g.last.offset[1] + e[1]) * (tetrimino_size + border_size) +
              border_size + outline_size + screen_width / 2 -
              min_screen_width / 2));
        pixels[index + 0] = c.r;
        pixels[index + 1] = c.g;
        pixels[index + 2] = c.b;
        pixels[index + 3] = 200;

        index =
            4 * (screen_width * (q +
                                 (g.last.offset[0] + e[0]) *
                                     (tetrimino_size + border_size) +
                                 border_size + outline_size +
                                 screen_height / 2 - min_screen_height / 2) +
                 ((tetrimino_size - 1 - p) +
                  (g.last.offset[1] + e[1]) * (tetrimino_size + border_size) +
                  border_size + outline_size + screen_width / 2 -
                  min_screen_width / 2));

        pixels[index + 0] = c.r;
        pixels[index + 1] = c.g;
        pixels[index + 2] = c.b;
        pixels[index + 3] = 200;
      }
    }

    for (int q = 0; q < border_size; ++q) {
      for (int p = 0; p < tetrimino_size; ++p) {
        int index =
            4 *
            (screen_width *
                 (q +
                  (g.last.offset[0] + e[0]) * (tetrimino_size + border_size) +
                  border_size + outline_size + screen_height / 2 -
                  min_screen_height / 2) +
             (p + (g.last.offset[1] + e[1]) * (tetrimino_size + border_size) +
              border_size + outline_size + screen_width / 2 -
              min_screen_width / 2));
        pixels[index + 0] = c.r;
        pixels[index + 1] = c.g;
        pixels[index + 2] = c.b;
        pixels[index + 3] = 200;

        index =
            4 *
            (screen_width *
                 ((tetrimino_size - 1 - q) +
                  (g.last.offset[0] + e[0]) * (tetrimino_size + border_size) +
                  border_size + outline_size + screen_height / 2 -
                  min_screen_height / 2) +
             (p + (g.last.offset[1] + e[1]) * (tetrimino_size + border_size) +
              border_size + outline_size + screen_width / 2 -
              min_screen_width / 2));

        pixels[index + 0] = c.r;
        pixels[index + 1] = c.g;
        pixels[index + 2] = c.b;
        pixels[index + 3] = 200;
      }
    }
  }
}

}  // namespace tetris