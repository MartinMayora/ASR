#include "../include/matrix.h"
#include "../include/vector.h"

class RNN {
public:
    // Architecture
    int input_size;
    int hidden_size;
    int output_size;
    int sequence_length;

    //W input weight matrix, U hidden layer weight matrix, V output layer weight matrix
    Matrix W, U, V; 
    //input vector
    Vector x;
    //output vector
    Vector y;
    //hidden state
    Vector h;

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
