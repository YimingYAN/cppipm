#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "Vector.h"
#include "Matrix.h"

/// Vector Operations
std::ostream &operator << (ostream& os, const Vector& rhs);
Vector operator + (const Vector& lhs, const Vector& rhs);
Vector operator - (const Vector& lhs, const Vector& rhs);
Vector operator * (const Vector& lhs, const Vector& rhs);
Vector operator / (const Vector& lhs, const Vector& rhs);
bool operator == (const Vector& lhs, const Vector& rhs);

/// Matrix Operations
std::ostream &operator << (ostream& os, const Matrix& rhs);
Matrix operator + (const Matrix& lhs, const Matrix& rhs);
Matrix operator - (const Matrix& lhs, const Matrix& rhs);
Matrix operator * (const Matrix& lhs, const Matrix& rhs);
Matrix operator * (const Matrix& lhs, const Vector& rhs);
Matrix operator * (const Vector& lhs, const Matrix& rhs);
bool operator == (const Matrix& lhs, const Matrix& rhs);

#endif // OPERATIONS_H
