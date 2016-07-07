#include "Vector.h"

Vector::Vector()
{
    m_data.resize(0);
}

Vector::Vector(const Vector &vec)
{
    this->m_data = vec.m_data;
}

Vector::Vector(int n)
{
    m_data.setConstant(n, 0.0);
}

Vector::Vector(int n, double value)
{
    m_data.setConstant(n, value);
}

Vector::Vector(int n, const double *array)
{
    if(n>0)
        m_data.resize(n);
    for(int i=0; i<n; i++)
        m_data(i) = array[i];
}

Vector::Vector(const std::vector<double> &vec)
{
    int n = vec.size();
    if(n == 0)
        return;

    m_data.resize(n);
    for(int i=0; i<n; i++)
        m_data(i) = vec[i];
}

Vector::~Vector()
{

}

std::size_t Vector::length() const
{
    return m_data.rows();
}

void Vector::resize(std::size_t n)
{
    m_data.resize(n);
}

void Vector::setEntry(std::size_t i, double val)
{
    m_data(i) = val;
}

void Vector::transpose()
{
    m_data.transpose();
}

Vector &Vector::operator =(const Vector &rhs)
{
    if(this == &rhs) {
        return *this;
    }

    this->m_data = rhs.m_data;
    return *this;
}

double Vector::operator()(std::size_t i) const
{
    return m_data(i);
}
