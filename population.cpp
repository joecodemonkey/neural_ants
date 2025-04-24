#include "population.hpp"
#include "raylib.h"
#include <algorithm>
#include <cmath> // Include this for std::ceil

void Population::draw() {
    for (auto &ant: _ants) { ant.draw(); }
}

void Population::smite() {
    for (Ant &ant: _ants) {
        const raylib::Vector2 &position = ant.get_position();
        if (position.x < 0 || position.x > _worldSize.x ||
            position.y < 0 || position.y > _worldSize.y) {
            ant.set_dead(true);
        }
    }
}

Ant Population::birth() {
    // The new ant will be within 20% of the bounds of the world
    auto x = GetRandomValue(static_cast<int>(std::round(_worldSize.x * 0.2)),
        static_cast<int>(std::round(_worldSize.x * 0.8)));

    auto y = GetRandomValue(static_cast<int>(std::round(_worldSize.y * 0.2)),
        static_cast<int>(std::round(_worldSize.y * 0.8)));

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

std::vector<std::reference_wrapper<Ant> > Population::find_touching(const Vector2 &position, float radius) {
    std::vector<std::reference_wrapper<Ant> > touching_ants;
    for (Ant &ant: _ants) {
        const float ant_radius = ant.get_size() / 2.0f;
        const float distance = Vector2Distance(position, ant.get_position());
        if (distance < (radius + ant_radius)) {
            touching_ants.push_back(ant);
        }
    }
    return touching_ants;
}
