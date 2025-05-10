#pragma once
#include <vector>
#include <raylib.h>
#include "neuron.hpp"


/*  Surroundings is a 2D grid of types that are encoded.
    The types are:
    - FOOD
    - EMPTY
    - WALL

    The top-left corner of the grid is (0,0)
    The bottom-right corner of the grid is (width-1, height-1)
    The grid is encoded left to right, top to bottom. 
    The encoded vector is a flat vector of values between -1 and 1.
*/


class Surroundings {

    public:

    typedef enum Type {
        FOOD,
        EMPTY,
        WALL
    } Type;

    Surroundings() = default;
    // copy constructor
    Surroundings(const Surroundings &other) = default;
    // move constructor
    Surroundings(Surroundings &&other) = delete;
    // copy assignment operator
    Surroundings &operator=(const Surroundings &other) = delete;
    // move assignment operator
    Surroundings &operator=(Surroundings &&other) = delete;

    void set_dimensions(size_t width, size_t height);
    void set_type(size_t x, size_t y, Type type);

    Vector2 get_dimensions() const;
    size_t get_width() const;
    size_t get_height() const;
    
    const std::vector<Neuron::Value> &get_encoded_surroundings();

    bool changed() const;

    protected:
    
    std::vector<std::vector<Type>> _surroundings_type;
    std::vector<Neuron::Value> _surroundings_encoded;

    bool _changed = false;

    static float encode_type(Type type);
    void update_encoded_surroundings();

};