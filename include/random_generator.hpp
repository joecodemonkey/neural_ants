#pragma once
#include <chrono>
#include <cmath>
#include <random>

#include "XoshiroCpp.hpp"

class RandomGenerator {
  mutable XoshiroCpp::Xoshiro256PlusPlus gen;

 public:
  // Default constructor with automatic random seeding
  RandomGenerator() : gen(std::random_device{}()) {}

  // Constructor with explicit seed for deterministic behavior
  explicit RandomGenerator(uint64_t seed) : gen(seed) {}

  double uniform(double min = 0.0, double max = 1.0) const {
    return std::uniform_real_distribution<double>(min, max)(gen);
  }

  int uniform_int(int min, int max) const {
    return std::uniform_int_distribution<int>(min, max)(gen);
  }

  double normal(double mean = 0.0, double stddev = 0.1) const {
    return std::normal_distribution<double>(mean, stddev)(gen);
  }

  bool coin_flip() {
    return uniform(0.0, 1.0) < 0.5;
  }
};