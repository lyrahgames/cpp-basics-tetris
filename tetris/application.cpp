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

application::application() {
  sf::ContextSettings settings;
  // settings.antialiasingLevel = 8;
  window.create(sf::VideoMode(500, 500), "Tetris", sf::Style::Default,
                settings);
  window.setVerticalSyncEnabled(true);
  // resize();

  view.setCenter({g.field.cols / 2, g.field.visible_rows / 2});
  view.setSize(window.getSize().x, window.getSize().y);
  // view.zoom(1 / cell_size);
  window.setView(view);
}

void application::execute() {
  while (window.isOpen()) {
    g.update();
    if (g.game_over)
      process_events_menu();
    else
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

      case sf::Event::MouseWheelMoved:
        cell_size *= exp(-event.mouseWheel.delta * 0.05f);
        cell_size = std::clamp(cell_size, min_cell_size, max_cell_size);
        resize();
        break;

      case sf::Event::Resized:
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

// folgendes müsste noch in die Funktion process_events integriert werden
void application::process_events_menu() {
  sf::Event event{};
  while (window.pollEvent(event)) {
    switch (event.type) {
      case sf::Event::Closed:
        window.close();
        break;

      case sf::Event::MouseWheelMoved:
        cell_size *= exp(-event.mouseWheel.delta * 0.05f);
        cell_size = std::clamp(cell_size, min_cell_size, max_cell_size);
        resize();
        break;

      case sf::Event::Resized:
        resize();
        break;

      case sf::Event::KeyPressed:
        switch (event.key.code) {
          case sf::Keyboard::Escape:
            window.close();
            break;

          case sf::Keyboard::Return:
            g.game_over = false;
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
        }
        break;
    }
  }
}

void application::render() {
  window.clear();
  if (g.game_over) {
    draw_menu();
  } else {
    draw_playfield();
    // draw_next();
    // draw_last();
    draw_text("Level", 0, 255, 255, g.field.cols * 1.25,
              g.field.visible_rows / 20, 1);
    draw_tetrimino();
  }
  window.display();
}

void application::resize() {
  view.setSize(window.getSize().x, window.getSize().y);
  view.zoom(1 / cell_size);
  window.setView(view);
}

void application::draw_playfield() {
  sf::RectangleShape tetrion{{g.field.cols + 2 * tetrion_border_size,
                              g.field.visible_rows + 2 * tetrion_border_size}};
  tetrion.setPosition(-tetrion_border_size, -tetrion_border_size);
  tetrion.setFillColor(sf::Color::Black);
  tetrion.setOutlineThickness(tetrion_outline_size);
  tetrion.setOutlineColor(sf::Color::Red);
  window.draw(tetrion);

  for (int i = 0; i < g.field.visible_rows; ++i) {
    for (int j = 0; j < g.field.cols; ++j) {
      sf::RectangleShape box{{1.0f, 1.0f}};
      // box.setOrigin(-0.05f, -0.05f);
      box.setPosition(j * 1.0f, (g.field.visible_rows - 1 - i) * 1.0f);
      box.setFillColor(label_color(g.field(i, j)));
      box.setOutlineThickness(-cell_outline_size);
      box.setOutlineColor({0, 0, 0});
      window.draw(box);
    }
  }
}

void application::draw_tetrimino() {
  const auto c = label_color(g.current.type);
  for (const auto& e : g.current.shape) {
    sf::RectangleShape box{{1.0f, 1.0f}};
    // box.setOrigin(-0.05f, -0.05f);
    box.setPosition(
        (g.current.offset[1] + e[1]) * 1.0f,
        (g.field.visible_rows - 1 - g.current.offset[0] - e[0]) * 1.0f);
    box.setFillColor(c);
    box.setOutlineThickness(-cell_outline_size);
    box.setOutlineColor({0, 0, 0});
    window.draw(box);
  }
}

void application::draw_next() {
  const auto c = label_color(g.next.type);

  // for (const auto& e : g.next.shape) {
  //   for (int q = 0; q < tetrimino_size; ++q) {
  //     for (int p = 0; p < tetrimino_size; ++p) {
  //       const int index =
  //           4 * (screen_width * (q +
  //                                (g.next.offset[0] + e[0]) *
  //                                    (tetrimino_size + border_size) +
  //                                border_size + outline_size +
  //                                screen_height / 2 - min_screen_height / 2) +
  //                (p +
  //                 (playfield::cols + g.next.offset[1] + e[1]) *
  //                     (tetrimino_size + border_size) +
  //                 2 * border_size + outline_size + screen_width / 2 -
  //                 min_screen_width / 2));
  //       pixels[index + 0] = c.r;
  //       pixels[index + 1] = c.g;
  //       pixels[index + 2] = c.b;
  //       pixels[index + 3] = c.a;
  //     }
  //   }
  // }
}

void application::draw_last() {
  const auto c = label_color(g.last.type);

  for (const auto& e : g.last.shape) {
    sf::RectangleShape box{
        {1.0f - 2 * cell_outline_size, 1.0f - 2 * cell_outline_size}};
    box.setOrigin(-cell_outline_size, -cell_outline_size);
    box.setPosition(
        (g.last.offset[1] + e[1]) * 1.0f,
        (g.field.visible_rows - 1 - g.last.offset[0] - e[0]) * 1.0f);
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineThickness(-ghost_cell_outline_size);
    box.setOutlineColor({c.r, c.g, c.b, 128});
    window.draw(box);
  }
}

void application::draw_text(std::string text_in, const int r, const int g,
                            const int b, float xposition, float yposition,
                            float fontsize) {
  sf::Font mainGameFont;
  mainGameFont.loadFromFile("UbuntuMono-R.ttf");
  sf::Text text;
  text.setString(text_in);
  text.setCharacterSize(fontsize);
  text.setFillColor(sf::Color(r, g, b));
  text.setFont(mainGameFont);
  sf::FloatRect textRect = text.getGlobalBounds();
  text.setOrigin(textRect.left + textRect.width / 2.0f,
                 textRect.top + textRect.height / 2.0f);
  text.setPosition(xposition, yposition);
  window.draw(text);
}

void application::draw_menu() {
  draw_text("difficulty", 0, 255, 255, g.field.cols / 2,
            g.field.visible_rows / 2, 15);
}

}  // namespace tetris
