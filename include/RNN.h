#include "../include/matrix.h"
#include "../include/vector.h"

class RNN {
public:
    // Architecture
    int input_size;
    int hidden_size;
    int output_size;
    int sequence_length;

    // Parameters
    Matrix Wxh, Whh, Why;
    Vector bh, by;

    // State
    Vector h;

    // Hyperparameters
    double learning_rate;
    std::string activation;

    // Buffers for training
    std::vector<Vector> hidden_states;
    std::vector<Vector> outputs;

    // Constructor
    RNN(int input_size, int hidden_size, int output_size, int sequence_length);

    // Forward prop
    std::vector<Vector> forward(const std::vector<Vector> &inputs);

    void tanh(Vector& v);

    // Backward prop 
    void backward(const std::vector<Vector> &inputs, const std::vector<Vector> &targets);

    // Helper functions
    Vector activate(const Vector &x);
    Vector activate_derivative(const Vector &x);
};
