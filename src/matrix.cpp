#include "../include/matrix.h"


void Matrix::fill(double value) {
    std::fill(data.begin(), data.end(), value);
}

Matrix Matrix::giveCopy() {
    Matrix copy(rows, cols);
    copy.data = data;
    return copy;
}

void Matrix::apply(const std::function<double(double)>& func) {
    for (auto &x : data) x = func(x);
}

void Matrix::multiplyConstant(double h){
    Matrix::apply([h](double x){return x*h;});
}

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

static Vector multiply(const Matrix& A, const Vector v) {
    if (A.cols != v.size()) {
        throw std::runtime_error("Matrix Vector dimension mismatch");
    }
    Vector result(A.rows);
    for (int r = 0; r < A.rows; r++) {
        double sum = 0.0;
        for (int c = 0; c < A.cols; c++) {
            sum += A(r, c) * v[c];
        }
        result[r] = sum;
    }
    return result;
}


void Matrix::add(const Matrix& other) {
    if (rows != other.rows || cols != other.cols) throw std::runtime_error("Dimension mismatch");
    for (int i = 0; i < rows * cols; ++i) data[i] += other.data[i];
}

void Matrix::add(double value) {
    for (auto &x : data) x += value;
}