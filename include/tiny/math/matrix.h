#ifndef __TINY_VECTOR_HPP__
#define __TINY_VECTOR_HPP__

#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <cmath>
#include <string>
#include <boost/operators.hpp>

namespace tiny
{
    namespace math {

        namespace internal
        {
            template<typename S, size_t L, size_t C>
            struct mlayout
            {
                template<typename... Ss>
                mlayout(const Ss... x) : data{x...} {}
                mlayout() {}
            private:
                S data[L*C];
            };

            template<typename S>
            struct mlayout<S,1,2>
            {
                mlayout(const S &x, const S &y) : x{x}, y{y} {}
                mlayout() {}
                S x, y;
            };

            template<typename S>
            struct mlayout<S,1,3>
            {
                mlayout(const S &x, const S &y, const S &z)  : x{x}, y{y}, z{z} {}
                mlayout() {}
                S x, y, z;
            };

            template<typename S>
            struct mlayout<S,1,4>
            {
                mlayout(const S &x, const S &y, const S &z, const S &w) : x{x}, y{y}, z{z}, w{w} {}
                mlayout() {}
                S x, y, z, w;
            };
        };

        template<typename S, size_t L, size_t C>
        struct matrix : internal::mlayout<S,L,C>,
                        boost::additive<matrix<S,L,C>,
                        boost::multiplicative2<matrix<S,L,C>, S,
                        boost::equality_comparable<matrix<S,L,C>,
                        boost::partially_ordered<matrix<S,L,C> > > > >
        {
            typedef matrix<S,L,C> mat;
            typedef matrix<S,C,L> tmat;
            typedef matrix<S,L,L> sqmat;
            typedef matrix<S,1,C> vec;
            typedef matrix<S,1,2> vec2;
            typedef matrix<S,1,3> vec3;
            typedef matrix<S,1,3> vec4;
            typedef internal::mlayout<S,L,C> mlayout;

            template<typename... Ss>
            explicit matrix(const Ss... x) : mlayout(x...) {}

            const S &operator()(size_t l, size_t c) const { return reinterpret_cast<const S*>(this)[l*C+c]; }
            const S &operator()(size_t i) const           { return (*this)(0,i); }

            S &operator()(size_t l, size_t c)             { return reinterpret_cast<S*>(this)[l*C+c]; }
            S &operator()(size_t i)                       { return (*this)(0,i); }

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

            bool operator==(const mat &r) const
            {
                for (size_t l = 0; l < lines(); ++l)
                    for (size_t c = 0; c < columns(); ++c)
                        if ((*this)(l,c) != r(l,c))
                            return false;
                return true;
            }

            bool operator<(const mat &r) const
            {
                for (size_t l = 0; l < lines(); ++l)
                    for (size_t c = 0; c < columns(); ++c)
                        if ((*this)(l,c) >= r(l,c))
                            return false;
                return true;
            }

            mat &operator+=(const mat &r)
            {
                for (size_t l = 0; l < lines(); ++l)
                    for (size_t c = 0; c < columns(); ++c)
                        (*this)(l,c) += r(l,c);
                return (*this);
            }

            mat &operator-=(const mat &r)
            {
                for (size_t l = 0; l < lines(); ++l)
                    for (size_t c = 0; c < columns(); ++c)
                        (*this)(l,c) -= r(l,c);
                return (*this);
            }

            mat &operator*=(const S &s)
            {
                for (size_t l = 0; l < lines(); ++l)
                    for (size_t c = 0; c < columns(); ++c)
                        (*this)(l,c) *= s;
                return (*this);
            }

            mat &operator/=(const S &s)
            {
                for (size_t l = 0; l < lines(); ++l)
                    for (size_t c = 0; c < columns(); ++c)
                        (*this)(l,c) /= s;
                return (*this);
            }

            template<size_t M>
            matrix<S,L,M> operator*(const matrix<S,C,M> &r) const
            {
                matrix<S,L,M> result;
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

            sqmat &operator*=(const sqmat &r)
            {
                (*this) = (*this) * r;
                return *this;
            }

            tmat transposed() const
            {
                tmat result;
                for (size_t l = 0; l < lines(); ++l)
                    for (size_t c = 0; c < columns(); ++c)
                        result(c,l) = (*this)(l,c);
                return result;
            }

            sqmat &transpose()
            {
                (*this) = this->transposed();
                return (*this);
            }

            vec3 operator^(const vec3 &r) const
            {
                const auto &l = (*this);
                return vec3( (l.y*r.z)-(r.y*l.z), (l.z*r.x)-(r.z*l.x), (l.x*r.y)-(r.x*l.y) );
            }

            S operator%(const vec &r) const
            {
                S accumulator = static_cast<S>(0.0);
                for (size_t i = 0; i < r.dimension(); ++i)
                    accumulator += (*this)(i) * r(i);
                return accumulator;
            }

            S length() const
            {
                return static_cast<S>(std::sqrt((*this) % (*this)));
            }

            vec &normalize()
            {
                (*this) /= length();
                return (*this);
            }

            vec normalized() const
            {
                auto result = (*this);
                return result.normalize();
            }

            vec &homogenize()
            {
                (*this) /= this->w;
                return (*this);
            }

            vec homogenized() const
            {
                vec result = (*this);
                return result.homogenize();
            }

            vec &scale(const vec &r)
            {
                for (size_t i = 0; i < r.dimension(); ++i)
                     (*this)(i) *= r(i);
                return (*this);
            }

            vec scaled(const vec &r)
            {
                vec result = (*this) ;
                return result.scale(r);
            }
        };

        template<typename vec>
        auto dot(const vec &l, const vec &r) -> decltype(l % r)
        {
            return l % r;
        }

        template<typename vec>
        auto cross(const vec &l, const vec &r) -> decltype(l ^ r)
        {
            return l ^ r;
        }

        template<typename vec>
        auto length(const vec &l) -> decltype(l.length())
        {
            return l.length();
        }

        template<typename S, size_t L, size_t C>
        std::ostream &operator<<(std::ostream &out, const matrix<S,L,C> &m)
        {
            size_t l, c;
            for (l = 0; l < L; ++l)
            {
                out << "(";
                for (c = 0; c < C-1; ++c)
                    out << m(l,c) << ",";
                out << m(l,c) << ")";

                if (l+1!=L) out << std::endl;
            }
            return out;
        }


        template<typename S, size_t N>
        using vector = matrix<S,1,N>;

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
    }
}

#endif
