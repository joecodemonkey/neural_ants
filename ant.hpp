#pragma once
#include <raylib-cpp.hpp>
#include <fmt/core.h>

class Ant {
public:
    Ant(raylib::Vector2 position, float speed, float direction)
        : _position(position), _speed(speed), _direction(direction), _dead(false) {
    }

    Ant &operator=(const Ant &other) {
        if (this != &other) {
            // Check for self-assignment
            _position = other._position;
            _speed = other._speed;
            _direction = other._direction;
            _dead = other._dead;
            _energy = other._energy;
            _lifeSpan = other._lifeSpan;
        }
        return *this;
    }

    Ant(): _position(0.0, 0.0), _speed(0), _direction(0.0), _dead(false) {
    }

    ~Ant() = default;

    void update();

    void draw();

    void set_dead(bool dead) { _dead = dead; }
    [[nodiscard]] bool is_dead() const { return _dead; }
    [[nodiscard]] float get_speed() const { return _speed; }
    [[nodiscard]] float get_direction() const { return _direction; }
    [[nodiscard]] float get_energy() const { return _energy; }
    void set_energy(float energy) { _energy = energy; }
    [[nodiscard]] float get_size() { return SIZE; }

    [[nodiscard]] const raylib::Vector2 &get_position() const { return _position; }

protected
:
    const float SIZE = 20.0f;

    raylib::Vector2 _position;
    float _speed;
    float _direction;
    bool _dead;

    float _energy = 10.0f;
    float _lifeSpan = 0.0f;
};
