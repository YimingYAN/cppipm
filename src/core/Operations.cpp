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
