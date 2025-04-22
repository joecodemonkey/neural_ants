#pragma once

#include "food.hpp"
#include <vector>
#include "population.hpp"

class Resources {
public:
    Resources() : _intended_food_count(100) {
    }

    explicit Resources(int size) : _intended_food_count(size) {
    }

    ~Resources() = default;

    void set_food_count(int size) { _intended_food_count = size; }

    void update();

    void draw();

    void feed_ants(Population &population);

protected:
    static Food new_food();

    size_t _intended_food_count = 0;
    std::vector<Food> _food;
};
