#pragma once

#include <algorithm>
#include <nlohmann/json.hpp>
#include <vector>

#include "genome.hpp"
#include "random_generator.hpp"

class Pangenome {
 public:
  static constexpr size_t MAX_PANGENOME_SIZE = 100;
  static constexpr size_t TOP_PERCENT_SIZE = 20;  // Top 20%
  static constexpr size_t MAX_CHILDREN_COUNT = 5;

  Pangenome() = default;
  Pangenome(const nlohmann::json& json);
  ~Pangenome() = default;

  // Copy and move operations
  Pangenome(const Pangenome& other) = default;
  Pangenome& operator=(const Pangenome& other) = default;
  Pangenome(Pangenome&& other) = default;
  Pangenome& operator=(Pangenome&& other) = default;

  auto add(Genome&& genome) -> void;
  auto sample_top_cycle() -> const Genome&;
  auto sample_random() -> const Genome&;
  auto size() const -> size_t;
  auto empty() const -> bool;
  auto to_json() const -> nlohmann::json;

  auto operator==(const Pangenome& other) const -> bool;

 private:
  std::vector<Genome> _genomes;  // Always sorted by fitness descending
  size_t _topCycleIndex = 0;
  mutable RandomGenerator _rng;

  auto maintain_sorted_order() -> void;
  auto increment_child_count(size_t index) -> void;
  auto find_insertion_point(double fitness) -> size_t;
};