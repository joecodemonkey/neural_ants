#pragma once
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <sstream>
#include "neuron.hpp"


// A fully connected neural network
class NeuralNetwork {

public:

    typedef std::vector<Neuron> Layer;
    typedef std::vector<Neuron::Value> ValueVector;

    NeuralNetwork() = default;
    
    void set_input(const ValueVector &input);
    void set_input(size_t idx, Neuron::Value value);
    
    const NeuralNetwork::ValueVector & get_inputs() const;
        
    Neuron::Value get_input(size_t idx) const;

    const ValueVector & get_outputs();
    Neuron::Value get_output(size_t idx);

    void set_input_layer_count(size_t count);
    size_t get_input_layer_count() const;

    void set_output_layer_count(size_t count);
    size_t get_output_layer_count() const;

    Layer & get_layer(size_t idx);
    const Layer & get_layer(size_t idx) const;

    void set_hidden_layer_count(size_t count);
    size_t get_hidden_layer_count() const;
    
    Layer & get_output_layer();
    const Layer & get_output_layer() const;

    void randomize();

    protected: 
    
    std::vector<Layer> _layers;   
    Layer _output_layer;
    ValueVector _inputs;
    ValueVector _outputs;

    bool _ready;
};
