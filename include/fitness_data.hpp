#include <vector>
class FitnessData {
 public:
  enum Resolution { SECOND = 0, MINUTE = 1, FIVE_MINUTE = 2, HOUR = 3 };
  typedef double DataValue;
  typedef double TimeValue;
  FitnessData();
  auto get_resolution_data(Resolution resolution) const -> const std::vector<DataValue>&;
  auto get_resolution_data(Resolution resolution) -> std::vector<DataValue>&;

  auto get_resolution_time(Resolution resolution) const -> const std::vector<TimeValue>&;
  auto get_resolution_time(Resolution resolution) -> std::vector<TimeValue>&;

  auto add_data(TimeValue time, DataValue dataPoint) -> void;

 protected:
  typedef struct _fitness {
    TimeValue time;
    DataValue fitness;
  } Event;

  auto aggregate(Resolution resolution) -> void;
  auto validate_raw_data() -> void;
  auto consume_raw_data() -> void;

  auto most_recent_raw_time() const -> double;
  auto most_recent_time() const -> double;

  std::vector<Event> _rawData;
  std::vector<std::vector<DataValue>> _data;
  std::vector<std::vector<TimeValue>> _time;
};