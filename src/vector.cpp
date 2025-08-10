#include "../include/vector.h"

void Vector::add(Vector vectorToAdd){
    if(this->size() != vectorToAdd.size()){
        throw std::invalid_argument("Sum of vectors of different sizes");
    }
    for(int i=0; i<vectorToAdd.size(); i++){
        this->data[i] += vectorToAdd[i];
    }
}

void Vector::multiplyMatrix(Matrix A) {
    if(this->size() != A.rows){
        throw std::invalid_argument("Multiplication vector - matrix invalid sizes")
    }
    std::vector<double> result(A.rows, 0.0);
    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            result[i] += A(i, j) * this->data[j];
        }
    }
    this->data = result;
}