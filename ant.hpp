#pragma once
#include <raylib-cpp.hpp>
#include <fmt/core.h>

class Ant {

public:

    Ant(raylib::Vector2 position, float speed, float direction)
        : _position(position), _speed(speed), _direction(direction) {}

    Ant(const Ant& other) = default;

    Ant& operator=(const Ant& other) = default;

    Ant(): _position(0.0, 0.0), _speed(0), _direction(0.0) {}
    ~Ant() = default;

    void update();
    void draw();

protected:
    raylib::Vector2 _position;
    float _speed;
    float _direction;
};