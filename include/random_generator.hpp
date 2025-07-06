#pragma once
#include <chrono>
#include <random>

class RandomGenerator {
  mutable std::mt19937 gen;

 public:
  RandomGenerator() : gen(std::chrono::steady_clock::now().time_since_epoch().count()) {}

  explicit RandomGenerator(std::mt19937::result_type seed) : gen(seed) {}

  void seed(std::mt19937::result_type seed) {
    gen.seed(seed);
  }

  void seed() {
    gen.seed(std::chrono::steady_clock::now().time_since_epoch().count());
  }

  double uniform(double min = 0.0, double max = 1.0) const {
    return std::uniform_real_distribution<double>(min, max)(gen);
  }
  double normal(double mean = 0.0, double stddev = 0.1) const {
    return std::normal_distribution<double>(mean, stddev)(gen);
  }
  bool coin_flip() {
    return uniform(0.0, 1.0) < 0.5;
  }
};