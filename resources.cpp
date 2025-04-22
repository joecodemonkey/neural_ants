#include "resources.hpp"
#include <algorithm>

void Resources::update() {
    if (_food.size() > _intended_food_count) {
        // stupidly inefficient, but it works and we don't care about performance right now
        std::remove_if(_food.begin(), _food.end(), [](const Food &food) { return food.is_eaten(); });
    }

    // Add food to the resources
    while (_food.size() < _intended_food_count) {
        _food.push_back(new_food());
    }

    // replace the food that has been eaten,
    // we should probably do this in a more efficient way
    for (Food &food: _food) {
        if (food.is_eaten()) {
            food = new_food();
        }
    }
}

void Resources::draw() {
    for (auto &food: _food) { food.draw(); }
}

Food Resources::new_food() {
    // The new food will be within 20% of the bounds of the screen
    auto screenWidth = GetScreenWidth();
    auto screenHeight = GetScreenHeight();
    auto x = GetRandomValue(screenWidth * 0.2, screenWidth * 0.8);
    auto y = GetRandomValue(screenHeight * 0.2, screenHeight * 0.8);
    return Food(raylib::Vector2(x, y));
}


void Resources::feed_ants(Population &population) {
    for (Food &food: _food) {
        auto ants = population.find_touching(food.get_position(), food.get_size());
        for (auto &ant: ants) {
            food.eat(ant.get());
            break; // Only one ant can eat the food at a time
        }
    }
}
