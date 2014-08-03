#ifndef __TINY_VECTOR_HPP__
#define __TINY_VECTOR_HPP__

#include <iostream>

namespace tiny {

template<typename S, int L, int C>
class matrix
{
public:
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
