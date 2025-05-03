#include "surroundings.hpp"
#include "neural_network.hpp"
#include "population.hpp"
#include "resources.hpp"
#include "raylib.h"
#include "genome.hpp"

#pragma once


class Brain {
    Brain() = default;

    void update(float delta_time);

    void set_population(Population *population);
    Population *get_population();
    
    void set_resources(Resources *resources);
    Resources *get_resources();
    
    float get_speed() const;    
    float get_direction() const;    

    void set_bounds(Rectangle &bounds);    
    Rectangle &get_bounds();
    const Rectangle &get_bounds() const;
    
    Genome &get_genome();
    const Genome &get_genome() const;
    void set_genome(const Genome &);

    Vector2 get_position() const;
    void set_position(Vector2 position);

    void set_tile_size(size_t size);
    size_t get_tile_size() const;
    void set_tile_window_size(size_t size);
    size_t get_tile_window_size() const;
        
    protected:

    float direction;
    float speed;

    void set_food();
    void set_ants();
    void set_empty();
    void set_wall();    

    Population *population = nullptr;
    Resources *resources = nullptr;

    size_t _tile_size;
    Surroundings _surroundings;
    Genome _genome;
    Rectangle _bounds;
};