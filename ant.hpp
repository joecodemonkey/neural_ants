#pragma once
#include <raylib-cpp.hpp>
#include <fmt/core.h>

class Ant {
public:
    Ant(raylib::Vector2 position, float speed, float direction)
        : _position(position), _speed(speed), _direction(direction), _dead(false) {
    }

    Ant(const Ant &other) = default;

    Ant &operator=(const Ant &other) = default;

    Ant(): _position(0.0, 0.0), _speed(0), _direction(0.0), _dead(false) {
    }

    ~Ant() = default;

    void update();

    void draw();

    void set_dead(bool dead) { _dead = dead; }
    [[nodiscard]] bool is_dead() const { return _dead; }
    [[nodiscard]] float get_speed() const { return _speed; }
    [[nodiscard]] float get_direction() const { return _direction; }
    [[nodiscard]] const raylib::Vector2 &get_position() const { return _position; }

protected:
    raylib::Vector2 _position;
    float _speed;
    float _direction;
    bool _dead;
};
