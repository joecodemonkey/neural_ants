#include "pangenome.hpp"

#include <algorithm>
#include <random>

#include "genome.hpp"

Pangenome::Pangenome(const nlohmann::json& json) {
  for (const auto& entryJson : json.at("genomes")) {
    Genome genome(entryJson.at("genome"));
    genome.set_children_count(entryJson.at("children_count").get<size_t>());
    _genomes.push_back(std::move(genome));
  }
  _topCycleIndex = json.at("top_cycle_index").get<size_t>();

  maintain_sorted_order();

  // Ensure _topCycleIndex is valid after loading
  if (_genomes.empty()) {
    _topCycleIndex = 0;
  } else if (_topCycleIndex >= _genomes.size()) {
    _topCycleIndex = 0;
  }
}

auto Pangenome::add(Genome&& genome) -> void {
  // Find insertion point to maintain sorted order (fitness descending)
  size_t insertionPoint = find_insertion_point(genome.get_fitness());

  // Bounds check insertion point to prevent massive allocations
  if (insertionPoint > _genomes.size()) {
    throw std::runtime_error("Invalid insertion point: " + std::to_string(insertionPoint) +
                             " for vector of size " + std::to_string(_genomes.size()));
  }

  _genomes.insert(_genomes.begin() + insertionPoint, std::move(genome));

  // Remove least fit genomes if we exceed max size
  while (_genomes.size() > MAX_PANGENOME_SIZE) {
    _genomes.pop_back();  // Remove from end (least fit)
  }
}

auto Pangenome::sample_top_cycle() -> const Genome& {
  if (_genomes.empty()) {
    throw std::runtime_error("Cannot sample from empty pangenome");
  }

  // Calculate the actual top percent size based on current genome count
  size_t actualTopSize = std::min(TOP_PERCENT_SIZE, _genomes.size());

  // Safety check: actualTopSize must be > 0
  if (actualTopSize == 0) {
    throw std::runtime_error("Internal error: actualTopSize is 0 in sample_top_cycle");
  }

  // Ensure _topCycleIndex is within bounds
  if (_topCycleIndex >= _genomes.size()) {
    _topCycleIndex = 0;
  }

  // Get the current genome in the cycle
  size_t index = _topCycleIndex % actualTopSize;
  
  // Store a static copy to return safely (since increment_child_count may remove the genome)
  static thread_local Genome selectedGenomeCopy;
  selectedGenomeCopy = _genomes[index];

  // Increment child count for this parent (may remove genome from vector)
  increment_child_count(index);

  // Recalculate actualTopSize after potential removal
  if (!_genomes.empty()) {
    actualTopSize = std::min(TOP_PERCENT_SIZE, _genomes.size());
    _topCycleIndex = (_topCycleIndex + 1) % actualTopSize;
  }

  return selectedGenomeCopy;
}

auto Pangenome::sample_random() -> const Genome& {
  if (_genomes.empty()) {
    throw std::runtime_error("Cannot sample from empty pangenome");
  }

  // Use project's random number generator
  int randomIndex = _rng.uniform_int(0, static_cast<int>(_genomes.size()) - 1);
  
  // Store a static copy to return safely (since increment_child_count may remove the genome)
  static thread_local Genome selectedGenomeCopy;
  selectedGenomeCopy = _genomes[randomIndex];

  // Increment child count for this parent (may remove genome from vector)
  increment_child_count(static_cast<size_t>(randomIndex));

  return selectedGenomeCopy;
}

auto Pangenome::size() const -> size_t {
  return _genomes.size();
}

auto Pangenome::empty() const -> bool {
  return _genomes.empty();
}

auto Pangenome::to_json() const -> nlohmann::json {
  nlohmann::json j;

  nlohmann::json genomesArray = nlohmann::json::array();
  for (const auto& genome : _genomes) {
    nlohmann::json entryJson;
    entryJson["genome"] = genome.to_json();
    entryJson["children_count"] = genome.get_children_count();
    genomesArray.push_back(entryJson);
  }

  j["genomes"] = genomesArray;
  j["top_cycle_index"] = _topCycleIndex;

  return j;
}

auto Pangenome::operator==(const Pangenome& other) const -> bool {
  if (_genomes.size() != other._genomes.size()) {
    return false;
  }

  for (size_t i = 0; i < _genomes.size(); ++i) {
    if (!(_genomes[i] == other._genomes[i])) {
      return false;
    }
  }

  return _topCycleIndex == other._topCycleIndex;
}

auto Pangenome::maintain_sorted_order() -> void {
  std::sort(_genomes.begin(), _genomes.end(), [](const Genome& a, const Genome& b) {
    return a.get_fitness() > b.get_fitness();  // Descending order
  });
}

auto Pangenome::increment_child_count(size_t index) -> void {
  if (index < _genomes.size()) {
    _genomes[index].increment_children_count();

    // Remove genome immediately if it reaches max children
    if (_genomes[index].get_children_count() >= MAX_CHILDREN_COUNT) {
      // Adjust _topCycleIndex if we're removing a genome before it
      if (index < _topCycleIndex && _topCycleIndex > 0) {
        _topCycleIndex--;
      } else if (index == _topCycleIndex && _genomes.size() > 0 &&
                 _topCycleIndex >= _genomes.size() - 1) {
        // If we're removing the current cycle index and it's at the end, reset to 0
        _topCycleIndex = 0;
      }

      _genomes.erase(_genomes.begin() + index);

      // After removal, ensure _topCycleIndex is still valid
      if (_genomes.empty()) {
        _topCycleIndex = 0;
      } else if (_topCycleIndex >= _genomes.size()) {
        _topCycleIndex = 0;
      }
    }
  }
}

auto Pangenome::find_insertion_point(double fitness) -> size_t {
  // Validate vector state before search
  if (_genomes.size() > 1000000) {  // Sanity check for corrupted state
    throw std::runtime_error("Genome vector size is suspiciously large: " +
                             std::to_string(_genomes.size()));
  }

  // Use std::lower_bound to find insertion point for descending order
  auto it = std::lower_bound(
      _genomes.begin(), _genomes.end(), fitness, [](const Genome& genome, double fitness) {
        return genome.get_fitness() > fitness;
      });

  // Validate the distance calculation
  auto distance = std::distance(_genomes.begin(), it);
  if (distance < 0) {
    throw std::runtime_error("Negative distance in find_insertion_point: " +
                             std::to_string(distance));
  }

  size_t insertionPoint = static_cast<size_t>(distance);
  if (insertionPoint > _genomes.size()) {
    throw std::runtime_error(
        "Insertion point exceeds vector size: " + std::to_string(insertionPoint) + " > " +
        std::to_string(_genomes.size()));
  }

  return insertionPoint;
}