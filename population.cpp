#include "population.hpp"
#include "raylib.h"
#include <algorithm>

void Population::draw() {
    for (auto &ant: _ants) { ant.draw(); }
}

void Population::smite() {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    for (Ant &ant: _ants) {
        const raylib::Vector2 &position = ant.get_position();
        if (position.x < 0 || position.x > screenWidth ||
            position.y < 0 || position.y > screenHeight) {
            ant.set_dead(true);
        }
    }
}

Ant Population::birth() {
    // The new ant will be within 20% of the bounds of the screen
    auto screenWidth = GetScreenWidth();
    auto screenHeight = GetScreenHeight();
    auto x = GetRandomValue(screenWidth * 0.2, screenWidth * 0.8);
    auto y = GetRandomValue(screenHeight * 0.2, screenHeight * 0.8);

    // The new ant will have a random speed and direction
    float speed = GetRandomValue(1, 20);
    float direction = GetRandomValue(0, 360);
    return Ant(raylib::Vector2(x, y), speed, direction);
}

void Population::reproduce() {
    // ensure the population vector matches the size of the population
    while (_ants.size() < _size) { _ants.push_back(birth()); }

    // if the population vector is larger than the size of the population, move live ants at the end over the dead ones
    while (_ants.size() > _size) {
        // stupidly inefficient, but it works and we don't care about performance right now
        auto it = std::remove_if(_ants.begin(), _ants.end(), [](const Ant &ant) { return ant.is_dead(); });
        _ants.erase(it, _ants.end());
    }

    // rebirth dead ants
    for (auto &ant: _ants) {
        if (ant.is_dead()) {
            ant = birth();
        }
    }
}

void Population::update() {
    // kill off ants that are out of bounds
    smite();

    // ensure population size is maintained
    reproduce();

    // update all ants
    for (Ant &ant: _ants) { ant.update(); }
}
