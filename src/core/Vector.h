#ifndef VECTOR_H
#define VECTOR_H

#include "global.h"
#include <Eigen/Dense>
#include <vector>

class DLL_EXPORT Vector
{
public:
    Vector();
    Vector(const Vector& vec);
    Vector(int n);
    Vector(int n, double value);
    Vector(int n, const double* array);
    Vector(const std::vector<double>& vec);

    ~Vector();

    Vector& operator = (const Vector& rhs);

    std::size_t length() const;
    void resize(std::size_t n);

    void setEntry(std::size_t i, double val);
    void transpose();

    double operator () (std::size_t i) const;
    friend Vector operator + (const Vector& lhs, const Vector& rhs);
    friend Vector operator - (const Vector& lhs, const Vector& rhs);
    friend Vector operator * (const Vector& lhs, const Vector& rhs);
    friend Vector operator / (const Vector& lhs, const Vector& rhs);

    double* rawData() const;


private:
    Eigen::VectorXd m_data;
};

#endif // VECTOR_H
