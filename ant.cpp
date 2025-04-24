#include "ant.hpp"
#include "raylib.h"

void Ant::update() {
    // Update the ant's position based on its speed and direction
    _position.x += _speed * cos(_direction) * GetFrameTime();
    _position.y += _speed * sin(_direction) * GetFrameTime();

    _energy -= energy_per_tick() * GetFrameTime();
    if (_energy <= 0) {
        _dead = true;
    }
}

void Ant::draw() {
    // Draw the ant at its current position
    DrawCircle(_position.x, _position.y, SIZE / 2, BLUE);
    // Draw a line to represent the direction
    DrawLine(_position.x, _position.y, _position.x + 20 * cos(_direction), _position.y + 20 * sin(_direction),
             raylib::Color::Red());
    // Draw a label with the ant's position
    std::string text = fmt::format("({:.2f}, {:.2f})", _position.x, _position.y);
    auto textSize = MeasureTextEx(GetFontDefault(), text.c_str(), 20, 1.0f);
    int textX = _position.x - textSize.x / 2;
    int textY = _position.y - textSize.y / 2 + 20;
    DrawText(text.c_str(), textX, textY, 10.0, BLACK);
}

float Ant::energy_per_tick() {
    // Calculate the energy consumed per tick based on speed and direction
    return _speed * SIZE * 0.1f; // Example: energy consumption is proportional to speed
}