#include <algorithm>
#include <array>
#include <cmath>
#include <concepts>
#include <limits>
#include <ranges>
#include <type_traits>

namespace Util {

template <typename T>
inline bool equal(T a, T b, T tolerance = std::numeric_limits<T>::epsilon()) {
  static_assert(std::is_floating_point_v<T>, "T must be a floating-point type");
  return std::abs(a - b) <= tolerance;
}

// C++23 array version using concepts and ranges
template <std::floating_point T, std::size_t N>
inline bool equal(const T (&a)[N],
                  const T (&b)[N],
                  T tolerance = std::numeric_limits<T>::epsilon()) {
  return std::ranges::all_of(std::views::iota(0uz, N),
                             [&](std::size_t i) { return equal(a[i], b[i], tolerance); });
}
}  // namespace Util
