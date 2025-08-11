#pragma once
#include <vector>
#include <functional>
#include <cmath>
#include <stdexcept>
#include "../include/vector.h"
class Matrix {
public:
    int rows, cols;
    std::vector<double> data;

    Matrix(int r, int c, double init = 0.0) 
        : rows(r), cols(c), data(r * c, init) {}

    // Access operators
    double& operator()(int r, int c) { return data[r * cols + c]; }
    const double& operator()(int r, int c) const { return data[r * cols + c]; }

    // Gives copy of the matrix
    Matrix giveCopy(){

    }
    
    // Fill with a value
    void fill(double value);

    // Apply function element-wise
    void apply(const std::function<double(double)>& func);

    //  Multiplies matrix by constant
    void multiplyConstant(double h);

    // Matrix-matrix multiplication
    static Matrix multiply(const Matrix& A, const Matrix& B);

    // Matrix-vector multiplication
    static std::vector<double> multiply(const Matrix& A, const Vector& v);

    // Add another matrix
    void add(const Matrix& other);

    // Add scalar
    void add(double value);
}
