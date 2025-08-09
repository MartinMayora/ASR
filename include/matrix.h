#pragma once
#include <vector>
#include <functional>
#include <cmath>
#include <stdexcept>

class Matrix {
public:
    int rows, cols;
    std::vector<double> data;

    Matrix(int r, int c, double init = 0.0) 
        : rows(r), cols(c), data(r * c, init) {}

    // Access operators
    double& operator()(int r, int c) { return data[r * cols + c]; }
    const double& operator()(int r, int c) const { return data[r * cols + c]; }

    // Fill with a value
    void fill(double value) {
        std::fill(data.begin(), data.end(), value);
    }

    // Apply function element-wise
    void apply(const std::function<double(double)>& func) {
        for (auto &x : data) x = func(x);
    }

    // Matrix-matrix multiplication
    static Matrix multiply(const Matrix& A, const Matrix& B) {
        Matrix C(A.rows, B.cols);
        for (int i = 0; i < A.rows; ++i) {
            for (int j = 0; j < B.cols; ++j) {
                double sum = 0.0;
                for (int k = 0; k < A.cols; ++k) {
                    sum += A(i, k) * B(k, j);
                }
                C(i, j) = sum;
            }
        }
        return C;
    }

    // Matrix-vector multiplication
    static std::vector<double> multiply(const Matrix& A, const std::vector<double>& v) {
        std::vector<double> result(A.rows, 0.0);
        for (int i = 0; i < A.rows; ++i) {
            for (int j = 0; j < A.cols; ++j) {
                result[i] += A(i, j) * v[j];
            }
        }
        return result;
    }

    // Add another matrix
    void add(const Matrix& other) {
        if (rows != other.rows || cols != other.cols) throw std::runtime_error("Dimension mismatch");
        for (int i = 0; i < rows * cols; ++i) data[i] += other.data[i];
    }

    // Add scalar
    void add(double value) {
        for (auto &x : data) x += value;
    }
};
