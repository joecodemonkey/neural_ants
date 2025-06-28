#include <algorithm>
#include <array>
#include <cmath>
#include <concepts>
#include <limits>
#include <ranges>
#include <type_traits>
#include <vector>

namespace Util {

template <typename T>
inline bool equal(T a, T b, T tolerance = std::numeric_limits<T>::epsilon()) {
  static_assert(std::is_floating_point_v<T>, "T must be a floating-point type");
  return std::abs(a - b) <= tolerance;
}

template <std::floating_point T>
inline bool equal(const std::vector<T>& a,
                  const std::vector<T>& b,
                  T tolerance = std::numeric_limits<T>::epsilon()) {
  if (a.size() != b.size()) {
    return false;
  }
  return std::ranges::all_of(std::views::iota(0uz, a.size()),
                             [&](std::size_t i) { return equal(a[i], b[i], tolerance); });
}
}  // namespace Util