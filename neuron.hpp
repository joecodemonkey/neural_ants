#pragma once
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <sstream>

class Neuron {
    public:
        
        typedef float Value;
        typedef std::vector<Value> ValueVector;

        Neuron() { }

        void set_input(size_t idx, Value value);
        Value get_input(size_t idx) const;

        void set_input_weight(size_t idx, Value weight);
        Value get_input_weight(size_t idx) const;

        void set_bias(double bias);
        Value get_bias() const;

        Value get_output();

        size_t get_input_count() const;
        void set_input_count(size_t);   

        Neuron& operator=(const Neuron& other);
        Neuron& operator=(Neuron&& other);
        Neuron(const Neuron& other);
        Neuron(Neuron&& other);        

        void randomize();

    protected:

        static Value activation_function(Value x) {    
            return tanh(x);
        }

        bool _ready = false;
        Value _value = 0;
        Value _bias = 0;
        ValueVector _weights;
        ValueVector _inputs;
    
};
