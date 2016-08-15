#include "Operations.h"

ostream &operator <<(ostream &os, const Vector &rhs)
{
    int n = rhs.length();
    os << "[ ";
    for(int i=0; i<n; ++i)
        os << rhs(i) << " ";
    os << "]'";
    return os;
}

Vector operator+(const Vector &rhs, const Vector &lhs)
{
    Vector res;
    res.m_data = rhs.m_data + lhs.m_data;
    return res;
}

Vector operator-(const Vector &rhs, const Vector &lhs)
{
    Vector res;
    res.m_data = rhs.m_data - lhs.m_data;
    return res;
}

Vector operator*(const Vector &lhs, const Vector &rhs)
{
    if(lhs.length() != rhs.length()) {
        return Vector();
    }
    Vector res(lhs.length());
    for(std::size_t i=0; i<lhs.length(); ++i) {
        res.setEntry(i,lhs(i)*rhs(2));
    }
    return res;
}

Vector operator / (const Vector &lhs, const Vector &rhs)
{
    if(lhs.length() != rhs.length()) {
        return Vector();
    }
    Vector res(lhs.length());
    for(std::size_t i=0; i<lhs.length(); ++i) {
        res.setEntry(i,lhs(i)/rhs(2));
    }
    return res;
}

bool operator ==(const Vector &lhs, const Vector &rhs)
{
    return lhs.m_data == rhs.m_data;
}

Matrix operator +(const Matrix &lhs, const Matrix &rhs)
{
    Matrix res;
    res.m_data = lhs.m_data + rhs.m_data;
    return res;
}

Matrix operator -(const Matrix &lhs, const Matrix &rhs)
{
    Matrix res;
    res.m_data = lhs.m_data - rhs.m_data;
    return res;
}

Matrix operator *(const Matrix &lhs, const Matrix &rhs)
{
    Matrix res;
    res.m_data = lhs.m_data * rhs.m_data;
    return res;
}

Matrix operator *(const Matrix &lhs, const Vector &rhs)
{
    Matrix res(rhs);
    res.m_data = lhs.m_data * res.m_data;
    return res;
}

Matrix operator *(const Vector &lhs, const Matrix &rhs)
{
    Matrix res(lhs);
    res.m_data = res.m_data * rhs.m_data;
    return res;
}

bool operator ==(const Matrix &lhs, const Matrix &rhs)
{
    return lhs.m_data == rhs.m_data;
}

ostream &operator <<(ostream &os, const Matrix &rhs)
{
    int n = rhs.cols();
    int m = rhs.rows();
    for(int i=0; i<m; ++i) {
        for(int j=0; j<n; ++j) {
            os << rhs(i, j) << " ";
        }
        os << endl;
    }
    return os;
}
