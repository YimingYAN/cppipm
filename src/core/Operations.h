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

/// Matrix Operations

#endif // OPERATIONS_H
