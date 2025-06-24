#include <algorithm>
#include <cstddef>
#include <fitness_data.hpp>
#include <numeric>
#include <stdexcept>
#include <utility>
#include <vector>

FitnessData::FitnessData() {
  // Initialize vectors for each resolution (4 resolutions: SECOND, MINUTE, FIVE_MINUTE, HOUR)
  _data.resize(4);
  _time.resize(4);
}

auto FitnessData::get_resolution_data(Resolution resolution) const
    -> const std::vector<DataValue>& {
  return _data[resolution];
}

auto FitnessData::get_resolution_data(Resolution resolution) -> std::vector<DataValue>& {
  return _data[resolution];
}

auto FitnessData::get_resolution_time(Resolution resolution) const
    -> const std::vector<TimeValue>& {
  return _time[resolution];
}

auto FitnessData::get_resolution_time(Resolution resolution) -> std::vector<TimeValue>& {
  return _time[resolution];
}

auto FitnessData::add_data(TimeValue time, DataValue data) -> void {
  _rawData.push_back({time, data});
  // Aggregate data for all resolutions
  aggregate(SECOND);
}

auto FitnessData::aggregate(Resolution resolution) -> void {
  /* TODO: FINISH */
}

auto FitnessData::most_recent_raw_time() const -> double {
  return _rawData.back().time;
}

auto FitnessData::most_recent_time() const -> double {
  return _time[SECOND].back();
}

auto FitnessData::consume_raw_data() -> void {
  /* TODO: FINISH */
}

// TODO: Get rid of this when you prove it's always monotonically increasing
auto FitnessData::validate_raw_data() -> void {
  if (_data.empty() || _data[Resolution::SECOND].empty()) {
    return;
  }

  const auto lastTime = _data[Resolution::SECOND].back();
  for (auto time : _rawData) {
    if (time <= lastTime) {
      throw std::runtime_error("Time must be monotonically increasing");
    }
  }
}
