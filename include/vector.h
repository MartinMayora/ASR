#include <iostream>
#include <vector>

#include "./matrix.h"

class Vector {
private:
public:
    std::vector<double> data;
    Vector(int size) : data(size, 0.0) {}

    int size() const { return data.size(); }

    double& operator[](int i) { return data[i]; }
    const double& operator[](int i) const { return data[i]; }
    void add(Vector vectorToAdd);
    void multiplyMatrix(Matrix matrix);
};
