#include "genome.hpp"
#include <random>
#include <algorithm>

void Genome::create_network() {
    _network.set_input_layer_count(_input_count);
    _network.set_output_layer_count(_output_count);
    _network.set_hidden_layer_count(_layer_count);
    
    
    for (size_t i = 0; i < _layer_count; ++i) {
        _network.get_layer(i).resize(_neuron_count);
    }        
        
    _ready = true;
}

Genome::Genome() {    
}

// Copy constructor
Genome::Genome(const Genome& other) 
    : _network(other._network),
      _ready(other._ready),
      _input_count(other._input_count),
      _output_count(other._output_count),
      _layer_count(other._layer_count),
      _neuron_count(other._neuron_count) {
}

// Move constructor
Genome::Genome(Genome&& other) noexcept
    : _network(std::move(other._network)),
      _ready(other._ready),
      _input_count(other._input_count),
      _output_count(other._output_count),
      _layer_count(other._layer_count),
      _neuron_count(other._neuron_count) {
}

// Copy assignment
Genome& Genome::operator=(const Genome& other) {
    if (this != &other) {
        _network = other._network;
        _ready = other._ready;
        _input_count = other._input_count;
        _output_count = other._output_count;
        _layer_count = other._layer_count;
        _neuron_count = other._neuron_count;
    }
    return *this;
}

// Move assignment
Genome& Genome::operator=(Genome&& other) noexcept {
    if (this != &other) {
        _network = std::move(other._network);
        _ready = other._ready;
        _input_count = other._input_count;
        _output_count = other._output_count;
        _layer_count = other._layer_count;
        _neuron_count = other._neuron_count;
    }
    return *this;
}

NeuralNetwork& Genome::get_network() {
    if (!_ready) {
        create_network();
    }
    return _network;
}

void Genome::breed_layer(const NeuralNetwork::Layer& parent1, 
                        const NeuralNetwork::Layer& parent2,
                        NeuralNetwork::Layer& child) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dis(0.0, 1.0);
    
    for (size_t neuron_idx = 0; neuron_idx < parent1.size(); ++neuron_idx) {
        const Neuron& parent1_neuron = parent1[neuron_idx];
        const Neuron& parent2_neuron = parent2[neuron_idx];
        Neuron& child_neuron = child[neuron_idx];
        
        // Randomly select weights from either parent
        for (size_t weight_idx = 0; weight_idx < parent1_neuron.get_input_count(); ++weight_idx) {
            Neuron::Value value = 0.0f;            
            if (dis(gen) < 0.5) {
                value = parent1_neuron.get_input_weight(weight_idx);
            } else {
                value = parent2_neuron.get_input_weight(weight_idx);
            }
            child_neuron.set_input_weight(weight_idx, value);
        }
        
        // Randomly select bias from either parent
        if (dis(gen) < 0.5) {
            child_neuron.set_bias(parent1_neuron.get_bias());
        } else {
            child_neuron.set_bias(parent2_neuron.get_bias());
        }
    }
}

Genome Genome::breed_with(const Genome& other, double mutation_rate) {
    validate_compatible(other);
    
    Genome child;
    
    // Ensure child has same structure as parents
    child.set_input_count(get_input_count());
    child.set_output_count(get_output_count());
    child.set_layer_count(get_layer_count());
    child.set_hidden_layer_neuron_count(get_hidden_layer_neuron_count());
    
    // Breed hidden layers
    for (size_t layer_idx = 0; layer_idx < get_layer_count(); ++layer_idx) {
        breed_layer(_network.get_layer(layer_idx),
                   other._network.get_layer(layer_idx),
                   child._network.get_layer(layer_idx));
    }
    
    // Breed output layer
    breed_layer(_network.get_output_layer(),
               other._network.get_output_layer(),
               child._network.get_output_layer());
    
    // Apply mutations
    child.mutate(mutation_rate);
    
    return child;
}

void Genome::mutate(double mutation_rate) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<double> dis(0.0, 1.0);
    static std::normal_distribution<double> gaussian(0.0, 0.1); // Small mutations
    
    // Mutate hidden layers
    for (size_t layer_idx = 0; layer_idx < get_layer_count(); ++layer_idx) {
        auto& layer = _network.get_layer(layer_idx);
        for (auto& neuron : layer) {
            // Mutate weights
            for (size_t weight_idx = 0; weight_idx < neuron.get_input_count(); ++weight_idx) {
                if (dis(gen) < mutation_rate) {
                    double current_weight = neuron.get_input_weight(weight_idx);
                    neuron.set_input_weight(weight_idx, current_weight + gaussian(gen));
                }
            }
            
            // Mutate bias
            if (dis(gen) < mutation_rate) {
                double current_bias = neuron.get_bias();
                neuron.set_bias(current_bias + gaussian(gen));
            }
        }
    }
    
    // Mutate output layer
    for (auto& neuron : _network.get_output_layer()) {
        // Mutate weights
        for (size_t weight_idx = 0; weight_idx < neuron.get_input_count(); ++weight_idx) {
            if (dis(gen) < mutation_rate) {
                double current_weight = neuron.get_input_weight(weight_idx);
                neuron.set_input_weight(weight_idx, current_weight + gaussian(gen));
            }
        }
        
        // Mutate bias
        if (dis(gen) < mutation_rate) {
            double current_bias = neuron.get_bias();
            neuron.set_bias(current_bias + gaussian(gen));
        }
    }
}

void Genome::randomize() {
    _network.randomize();
}

void Genome::set_layer_count(size_t count) {
    _layer_count = count;
    _ready = false;
}

size_t Genome::get_layer_count() const {
    return _layer_count;
}

void Genome::set_hidden_layer_neuron_count(size_t count) {
    _neuron_count = count;
    _ready = false;
}

size_t Genome::get_hidden_layer_neuron_count() const {
    return _neuron_count;
}

void Genome::set_input_count(size_t input_count) {
    _input_count = input_count;
    _ready = false;
}

size_t Genome::get_input_count() const {
    return _input_count;
}

void Genome::set_output_count(size_t output_count) {
    _output_count = output_count;
    _ready = false;
}

size_t Genome::get_output_count() const {
    return _output_count;
} 


void Genome::validate_compatible(const Genome &other) const {
    // Check if all network dimensions match
    if (_input_count != other._input_count) {
        throw std::invalid_argument("Input layer sizes do not match");
    }

    if (_output_count != other._output_count) {
        throw std::invalid_argument("Output layer sizes do not match"); 
    }

    if (_layer_count != other._layer_count) {
        throw std::invalid_argument("Hidden layer counts do not match");
    }

    if (_neuron_count != other._neuron_count) {
        throw std::invalid_argument("Hidden layer sizes do not match");
    }
}