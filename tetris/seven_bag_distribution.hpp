#pragma once
#include <algorithm>
#include <array>

namespace tetris {

struct seven_bag_distribution {
  using result_type = int;

  template <typename RNG>
  auto operator()(RNG&& rng) noexcept {
    if (index == 7) {
      bag = decltype(bag){{1, 2, 3, 4, 5, 6, 7}};
      std::shuffle(begin(bag), end(bag), std::forward<RNG>(rng));
      index = 0;
    }

    return bag[index++];
  }

  std::array<result_type, 7> bag{};
  size_t index{7};
};

}  // namespace tetris