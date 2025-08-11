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
    //NOT CLEAN CODE, CHANGE LATER ON PLS!
    for(int i = 1; i<inputs.size(); i++){
        // hi = g(Uhi-1 + Wxi)
        Vector z(this->U.rows);
        std::vector r = this->U.multiply(U,this->h[i-1]);
        Vector q(r.size());
        q.data = r;
        z.add(q);
        r =  this->W.multiply(W, inputs[i]);
        Vector q(r.size());
        q.data = r;
        z.add(q);
        RNN::tanh(z);
        this->h[i]=z;

        //yi = f(Vhi)
        std::vector 😭 = this->V.multiply(V, this->h[i]);
        Vector k(😭.size());
        k.data = 😭;
        RNN::softmax(k);
        y[i]=k;
    }
    return y;
}
