#ifndef __TINY_MATH_MATRIX_HPP__
#define __TINY_MATH_MATRIX_HPP__

#include <boost/operators.hpp>
#include "trigo.h"
#include <ostream>
#include <cmath>
#include <limits>
#include <cfloat>

namespace tiny { namespace math {

    namespace internal
    {
        template<typename scalar, size_t L, size_t C>
        struct default_layout
        {
            template<typename... scalars>
            default_layout(const scalars... x) : data{x...} {}
            default_layout() {}
        private:
            scalar data[L*C];
        };

        template<typename scalar>
        struct default_layout<scalar,2,1>
        {
            default_layout(const scalar &x, const scalar &y) : x{x}, y{y} {}
            default_layout(const scalar data[2]) : x{data[0]}, y{data[1]} {}
            default_layout() {}
            scalar x, y;
        };

        template<typename scalar>
        struct default_layout<scalar,3,1>
        {
            default_layout(const scalar &x, const scalar &y, const scalar &z) : x{x}, y{y}, z{z} {}
            default_layout(const default_layout<scalar,2,1> v, const scalar &z = scalar()) : x{v.x}, y{v.y}, z{z} {}
            default_layout(const scalar &x, const default_layout<scalar,2,1> v) : x{x}, y{v.x}, z{v.y} {}
            default_layout(const scalar data[3]) : x{data[0]}, y{data[1]}, z{data[2]} {}
            default_layout() {}
            scalar x, y, z;
        };

        template<typename scalar>
        struct default_layout<scalar,4,1>
        {
            default_layout(const scalar &x, const scalar &y, const scalar &z, const scalar &w) : x{x}, y{y}, z{z}, w{w} {}
            default_layout(const scalar &x, const default_layout<scalar,3,1> v) : x{x}, y{v.x}, z{v.y}, w{v.z} {}               
            default_layout(const default_layout<scalar,3,1> v, const scalar &w = scalar()) : x{v.x}, y{v.y}, z{v.z}, w{w} {}
            default_layout(const scalar data[4]) : x{data[0]}, y{data[1]}, z{data[2]}, w{data[3]} {}
            default_layout() {}
            scalar x, y, z, w;
        };
    };

    template<typename scalar, size_t L, size_t C, typename layout=internal::default_layout<scalar,L,C> >
    struct matrix : layout,
                    boost::additive<matrix<scalar,L,C>,
                    boost::additive2<matrix<scalar,L,C>, scalar,
                    boost::multiplicative2<matrix<scalar,L,C>, scalar,
                    boost::equality_comparable<matrix<scalar,L,C>,
                    boost::partially_ordered<matrix<scalar,L,C> > > > > >
    {
        typedef matrix<scalar,C,1> vec;
        typedef matrix<scalar,2,1> vec2;
        typedef matrix<scalar,3,1> vec3;
        typedef matrix<scalar,3,1> vec4;

        template<typename... scalars>
        matrix(const scalars... x) : layout{x...} {}

        const scalar &operator()(size_t l, size_t c) const { return reinterpret_cast<const scalar*>(this)[l*C+c]; }
        const scalar &operator()(size_t i) const           { return (*this)(0,i); }

        scalar &operator()(size_t l, size_t c)             { return reinterpret_cast<scalar*>(this)[l*C+c]; }
        scalar &operator()(size_t i)                       { return (*this)(0,i); }

        constexpr size_t lines() const                { return L; }
        constexpr size_t columns() const              { return C; }
        constexpr size_t dimension() const            { return L*C; }

        vec2 xy()  const                              { return vec2(this->x,this->y); }
        vec2 xz()  const                              { return vec2(this->x,this->z); }
        vec2 yz()  const                              { return vec2(this->y,this->z); }
        vec2 yx()  const                              { return vec2(this->y,this->x); }
        vec2 zx()  const                              { return vec2(this->z,this->x); }
        vec2 zy()  const                              { return vec2(this->z,this->y); }

        vec3 xyz() const                              { return vec3(this->x, this->y, this->z); }
        vec3 xzy() const                              { return vec3(this->x, this->z, this->y); }
        vec3 yxz() const                              { return vec3(this->y, this->x, this->z); }
        vec3 yzx() const                              { return vec3(this->y, this->z, this->x); }
        vec3 zxy() const                              { return vec3(this->z, this->x, this->y); }
        vec3 zyx() const                              { return vec3(this->z, this->y, this->x); }

        bool operator==(const matrix &r) const
        {
            constexpr static const scalar EPSILON = std::numeric_limits<scalar>::epsilon() * static_cast<scalar>(4.0);

            for (size_t l = 0; l < lines(); ++l)
                for (size_t c = 0; c < columns(); ++c)
                    if (fabs((*this)(l,c) - r(l,c)) > EPSILON)
                        return false;
            return true;
        }

        bool operator<(const matrix &r) const
        {
            for (size_t l = 0; l < lines(); ++l)
                for (size_t c = 0; c < columns(); ++c)
                    if ((*this)(l,c) >= r(l,c))
                        return false;
            return true;
        }

        matrix &operator+=(const matrix &r)
        {
            for (size_t l = 0; l < lines(); ++l)
                for (size_t c = 0; c < columns(); ++c)
                    (*this)(l,c) += r(l,c);
            return (*this);
        }

        matrix &operator-=(const matrix &r)
        {
            for (size_t l = 0; l < lines(); ++l)
                for (size_t c = 0; c < columns(); ++c)
                    (*this)(l,c) -= r(l,c);
            return (*this);
        }

        matrix &operator+=(const scalar &s)
        {
            for (size_t l = 0; l < lines(); ++l)
                for (size_t c = 0; c < columns(); ++c)
                    (*this)(l,c) += s;
            return (*this);
        }

        matrix &operator-=(const scalar &s)
        {
            for (size_t l = 0; l < lines(); ++l)
                for (size_t c = 0; c < columns(); ++c)
                    (*this)(l,c) -= s;
            return (*this);
        }

        matrix &operator*=(const scalar &s)
        {
            for (size_t l = 0; l < lines(); ++l)
                for (size_t c = 0; c < columns(); ++c)
                    (*this)(l,c) *= s;
            return (*this);
        }

        matrix &operator/=(const scalar &s)
        {
            for (size_t l = 0; l < lines(); ++l)
                for (size_t c = 0; c < columns(); ++c)
                    (*this)(l,c) /= s;
            return (*this);
        }

        matrix operator-() const
        {
            return -1 * (*this);
        }

        template<size_t M>
        matrix<scalar,L,M> operator*(const matrix<scalar,C,M> &r) const
        {
            matrix<scalar,L,M> result;
            for (size_t i = 0; i < result.lines(); ++i)
            {
                for (size_t j = 0; j < result.columns(); ++j)
                {
                    result(i,j) = 0;
                    for (size_t k = 0; k < columns(); ++k)
                        result(i,j) += (*this)(i,k) * r(k, j);
                }
            }
            return result;
        }

        matrix &operator*=(const matrix &r)
        {
            (*this) = (*this) * r;
            return *this;
        }
    };

    template<typename S, size_t N>
    using vector = matrix<S,N,1>;

    using byte = unsigned char;

    using byte2 = vector<byte,2>;
    using int2 = vector<int,2>;
    using float2 = vector<float,2>;
    using double2 = vector<double,2>;

    using byte3 = vector<byte,3>;
    using int3 = vector<int,3>;
    using float3 = vector<float,3>;
    using double3 = vector<double,3>;

    using byte4 = vector<byte,4>;
    using int4 = vector<int,4>;
    using float4 = vector<float,4>;
    using double4 = vector<double,4>;

    using byte2x2 = matrix<byte,2,2>;
    using int2x2 = matrix<int,2,2>;
    using float2x2 = matrix<float,2,2>;
    using double2x2 = matrix<double,2,2>;

    using byte3x3 = matrix<byte,3,3>;
    using int3x3 = matrix<int,3,3>;
    using float3x3 = matrix<float,3,3>;
    using double3x3 = matrix<double,3,3>;

    using byte4x4 = matrix<byte,4,4>;
    using int4x4 = matrix<int,4,4>;
    using float4x4 = matrix<float,4,4>;
    using double4x4 = matrix<double,4,4>;


    template<typename scalar, size_t L, size_t C>
    std::ostream &operator<<(std::ostream &out, const matrix<scalar,L,C> &m)
    {
        size_t l, c;
        for (l = 0; l < L; ++l)
        {
            for (c = 0; c < C; ++c)
                out << m(l,c) << " ";
            out << std::endl;
        }
        return out;
    }

    template<typename scalar, size_t N>
    std::ostream &operator<<(std::ostream &out, const vector<scalar,N> &v)
    {
        size_t i;
        out << "< ";
        for (i = 0; i < N; ++i)
            out << v(i) << " ";
        out << ">";
        return out;
    }

    template<typename scalar, size_t N>
    scalar dot(const vector<scalar,N> &l, const vector<scalar,N> &r)
    {
        scalar accumulator = static_cast<scalar>(0.0);
        for (size_t i = 0; i < N; i++)
            accumulator += l(i)*r(i);
        return accumulator;
    }

    template<typename scalar, size_t N>
    double length(const vector<scalar,N> &v)
    {
        return std::sqrt(dot<scalar,N>(v,v));
    }

    template<typename scalar, size_t N>
    vector<scalar,N> normalize(const vector<scalar,N> &v)
    {
        return v/length(v);
    }

    template<typename scalar>
    vector<scalar,3> cross(const vector<scalar,3> &l, const vector<scalar,3> &r)
    {
        return vector<scalar,3>( 
                    (l.y*r.z)-(r.y*l.z), 
                    (l.z*r.x)-(r.z*l.x),
                    (l.x*r.y)-(r.x*l.y) 
        );
    }

    template<typename scalar, size_t N>
    scalar trace(const matrix<scalar,N,N> &m)  
    {
        scalar accumulator = static_cast<scalar>(0.0);
        for (size_t i = 0; i < N; ++i)
            accumulator += m(i,i);
        return accumulator;
    }

    template<typename scalar, size_t N>
    scalar max(const vector<scalar,N> &m)  
    {
        auto result = m(0);
        for (size_t i = 1; i < N; ++i)
            result = std::max(result, m(i));
        return result;
    }

    template<typename scalar, size_t L, size_t C>
    matrix<scalar,C,L> transposed(const matrix<scalar,L,C> &m)
    {
        matrix<scalar,C,L> result;
        for (size_t l = 0; l < L; ++l)
            for (size_t c = 0; c < C; ++c)
                result(c,l) = m(l,c);
        return result;
    }

    template<typename T>
    T lerp(const T &start, const T &end, const double &percent)
    {
        return start + percent * (end-start);
    }

    template<typename T>
    T nlerp(const T &start, const T &end, const double &percent)
    {
        return normalize(lerp(start, end, percent));
    }

    template<typename T>
    T slerp(const T &start, const T &end, const double &percent)
    {
        double cosTheta = dot(start, end);
        radian theta(acos(cosTheta) * percent);
        T relative = normalize(((end - start) * cosTheta));

        return start * math::cos(theta) + relative * math::sin(theta);
    }

}}

#endif
