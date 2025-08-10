#include "../include/RNN.h"

void RNN::tanh(Vector& vector){
    for (auto& v: vector.data){
        v = std::tanh(v);
    }
}

std::vector<Vector> RNN::forward(const std::vector<Vector> &inputs){
    this->h[0]=0;
    for(int i = 0; i<inputs.size(); i++){
        Vector z(U.rows);
        z.add()
        RNN::tanh() 
        this->h[i]=
    }
}
