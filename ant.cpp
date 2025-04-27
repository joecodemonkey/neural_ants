#include "ant.hpp"
#include "raylib.h"

void Ant::update() {
    if (_dead) return; // he's not dead, he's just resting
    
    update_energy();
    update_position();
}

void Ant::update_position() {
    // Update the ant's position based on its speed and direction
    _position.x += _speed * cos(_direction) * GetFrameTime();
    _position.y += _speed * sin(_direction) * GetFrameTime();
}

void Ant::update_energy() {
    const auto time = GetFrameTime();

    // Consume kinetic energy
    _energy -= _speed * 0.1f * SIZE * time;

    // Consume stationary energy
    _energy -= SEDINTARY_ENERGY_PER_SECOND * time;

    // If the ant's energy is less than or equal to 0, set the ant to dead
    if (_energy <= 0.0f) {
        _energy = 0.0f;
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
    int textX = _position.x + SIZE * 1.5f - textSize.x / 2;
    int textY = _position.y - textSize.y / 2 + 20;
    DrawText(text.c_str(), textX, textY, 10.0, BLACK);

    // A line to represent the ant's energy
    
    const int lineX = textX;

    float energyPercentage = _energy / STARTING_ENERGY;
    int lineLength = static_cast<int>(std::round(textSize.x / 2.0f * energyPercentage));
    const int lineWidth = textSize.y / 4.0f;
    const int lineY = textY + lineWidth * 2;
    
    raylib::Color lineColor = raylib::Color::Green();
    if (_energy < 25) lineColor = raylib::Color::Red();
    else if (_energy < 50) lineColor = raylib::Color::Orange();
    else if (_energy < 75) lineColor = raylib::Color::Yellow();
    
    DrawRectangle(lineX, lineY, lineLength, lineWidth, lineColor);    
}