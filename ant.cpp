#include "ant.hpp"
#include "raylib.h"
#include <iostream>
#include <cmath>
#include <string>
#include <fmt/format.h>

using std::cout;
using std::endl;

void Ant::update()
{
    if (_dead) return; // he's not dead, he's just resting

    update_position();
    update_energy();

}

void Ant::update_position()
{
    const float time = GetFrameTime();
    if(time == 0.0f) return;

    auto old_position = _position;    
    _position.x += _speed * cosf(_direction) * time;
    _position.y += _speed * sinf(_direction) * time;
    _frozen = _position.x == old_position.x && _position.y == old_position.y;
    if(_frozen) {
        cout << "Frozen!\n";
        cout << "Position: " << _position.x << ", " << _position.y << endl;
        cout << "Old position: " << old_position.x << ", " << old_position.y << endl;
    }
}

void Ant::update_energy()
{
    const auto time = GetFrameTime();
    if(time == 0.0f) return;

    // Consume kinetic energy
    _energy -= _speed * 0.1f * SIZE * time;

    // Consume stationary energy
    _energy -= SEDINTARY_ENERGY_PER_SECOND * time;

    // If the ant's energy is less than or equal to 0, set the ant to dead
    if (_energy <= 0.0f)
    {
        _energy = 0.0f;
        _dead = true;
    }
}

void Ant::draw()
{
    if (_dead) return; // he's not dead, he's just resting
    draw_body();

    draw_direction();

    auto text_rect = draw_coordinates();

    draw_energy(text_rect);
}

void Ant::draw_body()
{
    Color color = _frozen ? BLUE : WHITE;
    if (_frozen) cout << "Frozen!\n";

    if (IsTextureValid(_antTexture))
    {
        auto direction = _direction * 180.0f / PI + 90.0f;
        DrawTextureEx(_antTexture, _position, direction, 1.0f, color);
    }
    else
    {
        DrawCircle(_position.x, _position.y, SIZE / 2, color);
    }
}

void Ant::draw_energy(Rectangle const &text_rect)
{
    const int lineX = text_rect.x;

    float energyPercentage = _energy / STARTING_ENERGY;
    int lineLength = static_cast<int>(roundf(text_rect.width / 2.0f * energyPercentage));
    const int lineWidth = static_cast<int>(roundf(text_rect.height / 4.0f));
    const int lineY = static_cast<int>(roundf(text_rect.y + text_rect.height / 2.0f));

    Color lineColor = GREEN;
    if (_energy < 25)
        lineColor = RED;
    else if (_energy < 50)
        lineColor = ORANGE;
    else if (_energy < 75)
        lineColor = YELLOW;

    DrawRectangle(lineX, lineY, lineLength, lineWidth, lineColor);
}

Rectangle Ant::draw_coordinates()
{
    std::string text = fmt::format("({:.2f}, {:.2f})", _position.x, _position.y);
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), 20, 1.0f);
    float textX = roundf(_position.x + SIZE * 1.5f - textSize.x / 2.0f);
    float textY = roundf(_position.y - textSize.y / 2.0f + 20);
    DrawText(text.c_str(), textX, textY, 10.0, BLACK);
    return {textX, textY, textSize.x, textSize.y};
}

Vector2 Ant::get_size()
{
    if (IsTextureValid(_antTexture))
    {
        return {(float)_antTexture.width, (float)_antTexture.height};
    }
    return {SIZE, SIZE};
}

void Ant::set_ant_texture_path(std::string const &path)
{

    _antTexture = LoadTexture(path.c_str());

    if (!IsTextureValid(_antTexture))
    {
        throw std::runtime_error("Failed to load ant texture at path: " + path);
    }

    _antTexturePath = path;
}

void Ant::draw_direction()
{
    DrawLine(
        _position.x + get_size().x / 2.0f,
        _position.y + get_size().y / 2.0f,
        _position.x + get_size().x / 2.0f + 20 * cosf(_direction),
        _position.y + get_size().y / 2.0f + 20 * sinf(_direction),
        RED
    );
}