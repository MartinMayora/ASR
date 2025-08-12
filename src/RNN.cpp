#include "../include/RNN.h"

void RNN::tanh(Vector& vector){
    for (auto& v: vector.data){
        v = std::tanh(v);
    }
}

void RNN::softmax(Vector& vector){
    double sum = 0;
    for(auto& v: vector.data){
        sum += exp(v);
    }
    if(0==sum){
        throw std::invalid_argument("empty vector in softmax");
    }
    for(auto& v: vector.data){
        v = exp(v)/sum;
    }
}

std::vector<Vector> RNN::forward(const std::vector<Vector> &inputs){
    this->h[0]=0;
    std::vector<Vector> y;
    y.reserve(inputs.size());
    for(int i = 1; i<inputs.size(); i++){
        // hi = g(Uhi-1 + Wxi)
        Vector z = Matrix::multiply(U, h[i - 1]);
        z.add(Matrix::multiply(W, inputs[i]));
        RNN::tanh(z);
        h[i] = z;

        // yi = softmax(V * hi)
        Vector k = Matrix::multiply(V, h[i]);
        RNN::softmax(k);
        y[i] = k;
    }
    return y;
}

BackwardPassResult RNN::lossFunction(std::vector<Vector> input, Vector target, double hp){
    std::vector<Vector> forwardY = forward(input);

    const double eps = 1e-12;

    double loss = 0.0;
    for (int i = 0; i < target.size(); ++i) {
        if (target[i] > 0.5) { // one-hot, so only 1 position matters
            loss = -std::log(std::max(y_last[i], eps));
            break;
        }
    }

    // Prepare result struct
    BackwardPassResult result;
    result.loss = loss;
    result.outputs = forwardY;  // store for backward pass if needed
    result.targets = target;
    return result;
}


void RNN::backward(const std::vector<Vector> &inputs, const std::vector<Vector> &targets){
    
}

