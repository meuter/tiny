#ifndef __TINY_VECTOR_HPP__
#define __TINY_VECTOR_HPP__

#include <iostream>
#include <initializer_list>
#include <stdexcept>

namespace tiny {

template<typename S, size_t L, size_t C>
class mdata
{
public:
    const S &operator()(size_t i, size_t j) const { return this->data[i][j]; }
    S &operator()(size_t i, size_t j)             { return this->data[i][j]; }
private:
    S data[L][C];
};

template<typename S>
class mdata<S,1,2>
{
public:
    S x, y;
};

template<typename S>
class mdata<S,1,3>
{
public:
    S x, y, z;
};

template<typename S>
class mdata<S,1,4>
{
public:
    S x, y, z, t;
};


template<typename S, size_t L, size_t C>
class matrix : public mdata<S,L,C>
{
public:
    matrix() {}

    matrix(const std::initializer_list<std::initializer_list<S>> &content)
    {
        S *ptr = reinterpret_cast<S*>(this);
        if (content.size() != lines())
            throw std::invalid_argument("expecting " + std::to_string(lines()) + " lines");
        for (const auto &line: content)
        {
            if (line.size() != columns())
                throw std::invalid_argument("expecting " + std::to_string(columns()) + " entries per column");
            for (const auto & cell: line)
                (*ptr++) = cell;
        }
    }

    constexpr size_t lines()   const      { return L; }
    constexpr size_t columns() const      { return C; }
};


template<typename S, size_t N>
class vector : public matrix<S,1,N>
{
public:
    vector(const std::initializer_list<S> &content)
    {
        S *ptr = reinterpret_cast<S*>(this);
        if (content.size() != this->columns())
            throw std::invalid_argument("expecting " + std::to_string(this->lines()) + " entries in vector");
        for (const auto & cell: content)
            (*ptr++) = cell;
    }
};


using int2 = vector<int,2>;
using float2 = vector<float,2>;

using int2x2 = matrix<int,2,2>;
using int3x3 = matrix<int,3,3>;
using int4x4 = matrix<int,4,4>;

using float2x2 = matrix<float,2,2>;
using float3x3 = matrix<float,3,3>;
using float4x4 = matrix<float,4,4>;
}


template<typename S>
std::ostream &operator<<(std::ostream &out, const tiny::vector<S,2> &v)
{
    return (out << "(" << v.x << "," << v.y << ")");
}


#endif
