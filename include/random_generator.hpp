#pragma once
#include <random>

class RandomGenerator {
  mutable std::mt19937 gen;

 public:
  double uniform(double min = 0.0, double max = 1.0) const {
    return std::uniform_real_distribution<double>(min, max)(gen);
  }
  double normal(double mean = 0.0, double stddev = 0.1) const {
    return std::normal_distribution<double>(mean, stddev)(gen);
  }
  bool coin_flip() {
    return uniform(0.0F, 1.0F) < 0.5F;
  }
};