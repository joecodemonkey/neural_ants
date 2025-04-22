#pragma once

#include "ant.hpp"
#include <vector>
#include <functional>

class Population {
    const int DEFAULT_POPULATION_SIZE = 100;

public:
    Population() = default;

    explicit Population(int size) : _size(size) {
    }

    ~Population() = default;

    void set_size(int size) { _size = size; }
    [[nodiscard]] int get_size() const { return _size; }

    void draw();

    void update();

    std::vector<std::reference_wrapper<Ant> > find_touching(const Vector2 &position, float radius);

    /* Explicitly obstruct any copying or moving of the Population object so that the compiler
     * will warn us if we try to do so. */

    Population(const Population &other) = delete;

    Population &operator=(const Population &other) = delete;

    Population(Population &&other) = delete;

    Population &operator=(Population &&other) = delete;

protected:
    static Ant birth();

    void smite();

    void reproduce();

    std::vector<Ant> _ants;
    int _size = DEFAULT_POPULATION_SIZE;
};
