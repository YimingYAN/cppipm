#ifndef MATRIX_H
#define MATRIX_H

#include "global.h"
#include <Eigen/Dense>
#include <vector>

class Matrix
{
public:
    Matrix();
    Matrix(std::size_t m, std::size_t n);
    Matrix(std::size_t m, std::size_t n, double val);
    Matrix(std::size_t m, std::size_t n, double* array);
    Matrix(std::size_t m, std::size_t n, std::vector<std::vector<double> > vec);
    ~Matrix();

    Matrix& operator = (const Matrix& rhs);
    friend Matrix operator + (const Matrix& lhs, const Matrix& rhs);
    friend Matrix operator - (const Matrix& lhs, const Matrix& rhs);
    friend Matrix operator * (const Matrix& lhs, const Matrix& rhs);

    void inverse();
    void transpose();

private:
    Eigen::MatrixXd m_data;

};

#endif // MATRIX_H
