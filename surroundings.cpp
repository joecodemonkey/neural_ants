#include "surroundings.hpp"

void Surroundings::set_dimensions(size_t width, size_t height) {
    // Resize the 2D vector to the specified dimensions
    _surroundings_type.resize(height);
    for (auto& row : _surroundings_type) {
        row.resize(width, EMPTY);  // Initialize all cells as EMPTY
    }
    
    // Resize the encoded vector to match the total number of cells
    _surroundings_encoded.resize(width * height);
    
    // Mark as changed since we've modified the structure
    _changed = true;
}

void Surroundings::set_type(size_t x, size_t y, Type type) {
    // Check bounds
    if (y < 0 || y >= _surroundings_type.size() || 
        x < 0 || x >= _surroundings_type[0].size()) {
        
        throw std::out_of_range("Index out of range");
    }
    
    // Set the type and mark as changed
    _surroundings_type[y][x] = type;

    if(_changed) return;
    
    // Update the encoded vector with this single change
    // to avoid re-encoding the entire vector
    _surroundings_encoded[y * get_width() + x] = encode_type(type);
}

Vector2 Surroundings::get_dimensions() const {
    if (_surroundings_type.empty()) {
        return {0, 0};
    }
    return {
        static_cast<float>(_surroundings_type[0].size()),
        static_cast<float>(_surroundings_type.size())
    };
}

void Surroundings::update_encoded_surroundings() {
   
    // Resize the encoded vector to match the total number of cells
    _surroundings_encoded.resize(get_width() * get_height());
    
    // Encode the surroundings
    size_t index = 0;
    for (const auto& row : _surroundings_type) {
        for (const auto& type : row) {
            _surroundings_encoded[index++] = encode_type(type);
        }
    }
    
    // Reset the changed flag
    _changed = false;
}

const std::vector<Neuron::Value> &Surroundings::get_encoded_surroundings() {
    // If the surroundings haven't changed, return the cached encoded vector
    if (!_changed) {
        return _surroundings_encoded;
    }

    update_encoded_surroundings();
    
    return _surroundings_encoded;
}

size_t Surroundings::get_width() const {
    if(_surroundings_type.empty()) return 0;

    return _surroundings_type[0].size();
}

size_t Surroundings::get_height() const {
    return _surroundings_type.size();
}

bool Surroundings::changed() const {
    return _changed;
}

float Surroundings::encode_type(Type type) {
    switch (type) {
        case FOOD:
            return 1.0f;
        case EMPTY:
            return 0.0f;
        case WALL:
            return -1.0f;
        default:
            return 0.0f;  // Default case for safety
    }
} 