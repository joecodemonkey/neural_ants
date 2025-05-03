#pragma once
#include <vector>
#include <raylib.h>

class Surroundings {

    public:

    typedef enum Type {
        FOOD,
        ANT,
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

    void set_dimensions(int width, int height);
    void set_type(int x, int y, Type type);
    Vector2 get_dimensions() const;

    void update();
    const std::vector<float> &get_encoded_surroundings() const;

    protected:
    
    std::vector<std::vector<Type>> _surroundings_type;
    std::vector<float> _surroundings_encoded;

    float encode_type(Type type) {
        switch (type) {
            case FOOD:
                return 1.0f;            
            case ANT:
                return -0.5f;
            case EMPTY:
                return 0.0f;
            case WALL:
                return -1.0f;
            default:
                return 0.0f;  // Default case for safety
        }
    }  
};