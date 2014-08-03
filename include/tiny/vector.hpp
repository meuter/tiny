#ifndef __TINY_VECTOR_HPP__
#define __TINY_VECTOR_HPP__

#include <iostream>
#include <initializer_list>
#include <stdexcept>

namespace tiny {

template<typename S, size_t L, size_t C>
class matrix
{
public:
    matrix() {}

    matrix(const std::initializer_list<std::initializer_list<S>> &content)
    {
        S *ptr = reinterpret_cast<S*>(&data[0][0]);
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

    matrix(const std::initialize_list<matrix<S,1,C>> &content)
    {
        S *ptr = reinterpret_cast<S*>(&data[0][0]);
        if (content.size() != lines())
            throw std::invalid_argument("expecting " + std::to_string(lines()) + " lines");
        for (const auto &line: content)
        {
            for (const auto & cell: line.data[0])
                (*ptr++) = cell;
        }

    }


    const S &operator()(size_t i, size_t j) const { return data[i][j]; }
    S &operator()(size_t i, size_t j)             { return data[i][j]; }

    constexpr size_t lines()   const      { return L; }
    constexpr size_t columns() const      { return C; }
private:
    S data[L][C];
};



template<typename S>
class vec2
{
public:
    vec2(const S &x, const S &y) : x(x), y(y) {}
    S x;
    S y;
};

using int2 = vec2<int>;
using float2 = vec2<float>;

using int2x2 = matrix<int,2,2>;
using int3x3 = matrix<int,3,3>;
using int4x4 = matrix<int,4,4>;

using float2x2 = matrix<float,2,2>;
using float3x3 = matrix<float,3,3>;
using float4x4 = matrix<float,4,4>;

}

template<typename S>
std::ostream &operator<<(std::ostream &out, const tiny::vec2<S> &v)
{
    return (out << "(" << v.x << "," << v.y << ")");
}


#endif
