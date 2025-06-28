#include <cmath>
#include <limits>
#include <type_traits>

namespace Util {

template <typename T>
inline bool equal(T a, T b, T tolerance = std::numeric_limits<T>::epsilon()) {
  static_assert(std::is_floating_point_v<T>, "T must be a floating-point type");
  return std::abs(a - b) <= tolerance;
}

}  // namespace Util
