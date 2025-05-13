#pragma once
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <sstream>
#include "neural_network.hpp"

class Genome {  

public:
    // Default constructor
    Genome();
    
    // Copy constructor
    Genome(const Genome& other);
    
    // Move constructor
    Genome(Genome&& other) noexcept;
    
    // Copy assignment
    Genome& operator=(const Genome& other);
    
    // Move assignment
    Genome& operator=(Genome&& other) noexcept;

    Genome breed_with(const Genome &other, double mutation_rate = 0.1);

    NeuralNetwork &get_network();

    void mutate(double mutation_rate = 0.1);
    void randomize();       

    void set_layer_count(size_t count);
    size_t get_layer_count() const;

    void set_hidden_layer_neuron_count(size_t count);
    size_t get_hidden_layer_neuron_count() const;

    void set_input_count(size_t input_count);
    size_t get_input_count() const;

    void set_output_count(size_t output_count);
    size_t get_output_count() const;

    void validate_compatible(const Genome &other) const;

protected:
    static void breed_layer(const NeuralNetwork::Layer& parent1, 
                            const NeuralNetwork::Layer& parent2,
                            NeuralNetwork::Layer& child);

    void create_network();

    NeuralNetwork _network;
    bool _ready = false;
    
    // Configuration settings
    size_t _input_count = 8;
    size_t _output_count = 3;
    size_t _layer_count = 1;
    size_t _neuron_count = 5;
};
