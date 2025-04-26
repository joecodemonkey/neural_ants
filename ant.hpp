#pragma once
#include <raylib-cpp.hpp>
#include <fmt/core.h>

class Ant {
public:
    Ant() { };

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
    
    ~Ant() = default;

    void update();

    void draw();


    [[nodiscard]] float get_direction() const { return _direction; }
    void set_direction(float direction) { _direction = direction; }

    [[nodiscard]] float get_energy() const { return _energy; }
    void set_energy(float energy) { _energy = energy; }

    [[nodiscard]] float get_speed() const { return _speed; }
    void set_speed(float speed) { _speed = speed; }
       
    [[nodiscard]] bool is_dead() const { return _dead; }
    void set_dead(bool dead) { _dead = dead; }
    
    [[nodiscard]] float get_life_span() const { return _lifeSpan; }

    [[nodiscard]] const raylib::Vector2 &get_position() const { return _position; }
    void set_position(raylib::Vector2 position) { _position = position; }

    [[nodiscard]] float const get_size() const { return SIZE; }
       
protected:
    
    const float SIZE = 20.0f;

    raylib::Vector2 _position = {0.0f, 0.0f};
    float _speed = 0.0f;
    float _direction = 0.0f;
    bool _dead = false;
    
    // All Ants start with the same amount of energy
    const float STARTING_ENERGY = 100.0f;
    float _energy = STARTING_ENERGY;

    // Lifespan records the number of seconds the ant has been alive
    float _lifeSpan = 0.0f;

    // sedintary energy per second is the base rate of energy loss for a stationary ant
    const float SEDINTARY_ENERGY_PER_SECOND = 0.1f;
    
    void update_energy();
    void update_position();
};
