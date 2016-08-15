#ifndef MATRIX_H
#define MATRIX_H

#include "global.h"
#include <Eigen/Dense>
#include <vector>

class Vector;

class Matrix
{
public:
    Matrix();
    Matrix(const Vector& vec);
    Matrix(std::size_t m, std::size_t n);
    Matrix(std::size_t m, std::size_t n, double val);
    Matrix(std::size_t m, std::size_t n, double* array);
    Matrix(std::size_t m, std::size_t n, const std::vector<std::vector<double> >& vec);
    Matrix(const std::vector<std::vector<double> >& vec);
    ~Matrix();

    Matrix& operator = (const Matrix& rhs);
    double operator() (std::size_t i, std::size_t j) const;
    friend Matrix operator + (const Matrix& lhs, const Matrix& rhs);
    friend Matrix operator - (const Matrix& lhs, const Matrix& rhs);
    friend Matrix operator * (const Matrix& lhs, const Matrix& rhs);
    friend Matrix operator * (const Matrix& lhs, const Vector& rhs);
    friend Matrix operator * (const Vector& lhs, const Matrix& rhs);
    friend bool operator == (const Matrix& lhs, const Matrix& rhs);

    std::size_t rows() const;
    std::size_t cols() const;

    void inverse();
    void transpose();

private:
    Eigen::MatrixXd m_data;

};

#endif // MATRIX_H
