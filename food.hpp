#pragma once
#include <raylib-cpp.hpp>
#include "ant.hpp"

class Food {
public:
    Food() : _value(10.0f), _position(0.0f, 0.0f), _eaten(false) {
    }

    Food(const raylib::Vector2 &position) : _value(10.0f), _position(position), _eaten(false) {
    }

    Food &operator=(const Food &other) {
        if (this != &other) {
            // Check for self-assignment
            _position = other._position;
            _value = other._value;
            _eaten = other._eaten;
        }
        return *this;
    }

    bool is_eaten() const { return _eaten; }

    void eat(Ant &ant) {
        if (_eaten) return;
        _eaten = true;
        ant.set_energy(ant.get_energy() + _value);
    }

    [[nodiscard]] const raylib::Vector2 &get_position() const { return _position; }

    void draw() {
        if (_eaten) return;
        DrawCircle(_position.x, _position.y, _size / 2, GREEN);
    }

    [[nodiscard]] float get_size() const { return _size; }

    [[nodiscard]] float get_value() const { return _value; }

protected:
    float _value;
    raylib::Vector2 _position;
    bool _eaten;
    const float _size = 10.0f;
};
