#include "Matrix.h"
#include <core/Vector.h>

Matrix::Matrix()
{

}

Matrix::Matrix(const Vector &vec)
{
    this->m_data.resize(1, vec.length());
    for(std::size_t i=0; i<vec.length(); ++i) {
        this->m_data(0,i) = vec(i);
    }
}

Matrix::Matrix(std::size_t m, std::size_t n, double val)
{
    m_data.resize(m,n);
    m_data.fill(val);
}

Matrix::Matrix(std::size_t m, std::size_t n,
               const std::vector<std::vector<double> >& vec)
{
    m_data.resize(m, n);
    for(std::size_t i=0; i<vec.size(); i++) {
        for(std::size_t j=0; j<vec[i].size(); j++) {
            m_data(i,j) = vec[i][j];
        }
    }
}

Matrix::Matrix(const std::vector<std::vector<double> > &vec)
{
    std::size_t m = vec.size();
    if(m > 0) {
        std::size_t n = vec[0].size();
        if(std::find_if(vec.cbegin()+1, vec.cend(),
                        [n](const vector<double>& inner) {return inner.size() != n;}) == std::end(vec)) {
            m_data.resize(m, n);
            for(std::size_t i=0; i<vec.size(); i++) {
                for(std::size_t j=0; j<vec[i].size(); j++) {
                    m_data(i,j) = vec[i][j];
                }
            }
        }
    }
}

Matrix::~Matrix()
{
    m_data.resize(0,0);
}

double Matrix::operator()(std::size_t i, std::size_t j) const
{
    return m_data(i, j);
}

std::size_t Matrix::rows() const
{
    return m_data.rows();
}

std::size_t Matrix::cols() const
{
    return m_data.cols();
}
