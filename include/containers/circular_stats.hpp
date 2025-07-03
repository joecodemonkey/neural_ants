#pragma once
#include <concepts>
#include <deque>
#include <iostream>
#include <numeric>

namespace Containers {
template <typename T>
concept Summable = requires(T a, T b) {
  { a + b } -> std::same_as<T>;
  { a += b } -> std::same_as<T&>;
  { T{} };
  { a / std::size_t{1} } -> std::convertible_to<T>;  // For mean calculation
  { a = 0 };                                         // can set to 0
};

template <Summable T>
class CircularStats {
 public:
  CircularStats();

  auto add_data(T dataPoint) -> void;

  auto get_mean() -> T;
  auto get_seen() -> size_t;

  auto set_max_data_points(size_t maxDataPoints) -> void;
  auto get_max_data_points() -> size_t;
  auto clear() -> void;

 protected:
  auto resize() -> void;
  auto refresh_mean() -> void;
  const size_t DEFAULT_MAX_DATA_POINTS = 100;
  size_t _seen;
  T _meanFitness;
  T _maxDataPoints;
  std::deque<T> _rawData;
};

template <Summable T>
auto CircularStats<T>::resize() -> void {
  while (_rawData.size() > _maxDataPoints) {
    _rawData.pop_front();
  }
}

template <Summable T>
auto CircularStats<T>::refresh_mean() -> void {
  if (_rawData.size() == 0) {
    _meanFitness = T{};
  } else {
    auto sum = std::reduce(_rawData.begin(), _rawData.end(), T{});
    _meanFitness = sum / static_cast<std::size_t>(_rawData.size());
  }
}

// Template implementation
template <Summable T>
CircularStats<T>::CircularStats()
    : _seen(0), _meanFitness(0), _maxDataPoints(DEFAULT_MAX_DATA_POINTS) {}

template <Summable T>
auto CircularStats<T>::add_data(T dataPoint) -> void {
  _rawData.push_back(dataPoint);
  ++_seen;
  resize();
  refresh_mean();
}

template <Summable T>
auto CircularStats<T>::get_mean() -> T {
  return _meanFitness;
}

template <Summable T>
auto CircularStats<T>::get_seen() -> size_t {
  return _seen;
}

template <Summable T>
auto CircularStats<T>::set_max_data_points(size_t maxDataPoints) -> void {
  _maxDataPoints = maxDataPoints;
  resize();
  refresh_mean();
}

template <Summable T>
auto CircularStats<T>::get_max_data_points() -> size_t {
  return _maxDataPoints;
}

template <Summable T>
auto CircularStats<T>::clear() -> void {
  _meanFitness = 0;
  _seen = 0;
  _rawData.clear();
}

}  // namespace Containers