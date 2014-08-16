#ifndef __TINY_VECTOR_HPP__
#define __TINY_VECTOR_HPP__

#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <cstdint>
#include <cmath>
#include <boost/operators.hpp>

namespace tiny
{
    namespace math {

        namespace internal
        {
            template<typename S, size_t L, size_t C>
            class matrix_data
            {
            private:
                S data[L][C];
            };

            template<typename S>
            class matrix_data<S,1,2>
            {
            public:
                union {
                    struct { S r, i; };
                    struct { S x, y; };
                };
            };

            template<typename S>
            class matrix_data<S,1,3>
            {
            public:
                union {
                    struct { S x, y, z; };
                    struct { S r, g, b; };
                };
            };

            template<typename S>
            class matrix_data<S,1,4>
            {
            public:
                union {
                    struct { S x, y, z, w; };
                    struct { S r, g, b, a; };
                };
            };
        };


        template<typename S, size_t L, size_t C>
        class matrix : public internal::matrix_data<S,L,C>,
                       private boost::additive<matrix<S,L,C>,
                               boost::multiplicative2<matrix<S,L,C>, S,
                               boost::equality_comparable<matrix<S,L,C> > > >
        {
            typedef matrix<S,L,C> mat;

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

            const S &operator()(size_t l, size_t c) const
            {
                return *( reinterpret_cast<const S*>(this)  + (l*C) + c );
            }

            S &operator()(size_t l, size_t c)
            {
                return *( reinterpret_cast<S*>(this)  + (l*C) + c );
            }

            constexpr size_t lines() const
            {
                return L;
            }

            constexpr size_t columns() const { return C;}

            constexpr matrix<S,1,2> dimensions() const
            {
                return matrix<S,1,2>{ { L, C } };
            }

            bool operator==(const mat &r) const
            {
                for (size_t l = 0; l < lines(); ++l)
                    for (size_t c = 0; c < columns(); ++c)
                        if ((*this)(l,c) != r(l,c))
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

            mat &operator*=(const mat &r)
            {
                (*this) = (*this) * r;
                return *this;
            }



        };

        template<typename S, size_t N>
        class vector;


        template<typename S>
        vector<S,3> cross(const vector<S,3> &l, const vector<S,3> &r)
        {
            return vector<S,3>{ (l.y*r.z)-(r.y*l.z), (l.z*r.x)-(r.z*l.x), (l.x*r.y)-(r.x*l.y) };
        }


        template<typename S, size_t N>
        class vector : public matrix<S,1,N>
        {
            typedef matrix<S,1,N> mat;
            typedef vector<S,N> vec;
            typedef vector<S,2> vec2;
            typedef vector<S,3> vec3;
        public:
            vector() {}
            vector(const vec &r) : mat{r} {}
            vector(const mat &r) : mat{r} {}
            vector(const std::initializer_list<S> &content) : mat{content} {}

            const S &operator()(size_t i) const
            {
                return *( reinterpret_cast<const S*>(this) + i );
            }

            S &operator()(size_t i)
            {
                return *( reinterpret_cast<S*>(this) + i );
            }

            constexpr size_t dimension() const
            {
                return N;
            }


            S dot(const vec &r) const
            {
                S accumulator = 0;
                for (size_t i = 0; i < N; ++i)
                    accumulator += (*this)(i) * r(i);
                return accumulator;
            }

            S operator%(const vec &r) const
            {
                return (this->dot(r));
            }

            auto length() const -> decltype(std::sqrt(dot(*this)))
            {
                return std::sqrt(this->dot(*this));
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

            vec cross(const vec &r) const
            {
                return tiny::math::cross(*this, r);
            }

            vec operator^(const vec &r) const
            {
                return this->cross(r);
            }

            vec2 xy()  const { return vec2{this->x,this->y}; }
            vec2 xz()  const { return vec2{this->x,this->z}; }
            vec2 yz()  const { return vec2{this->y,this->z}; }
            vec2 yx()  const { return vec2{this->y,this->x}; }
            vec2 zx()  const { return vec2{this->z,this->x}; }
            vec2 zy()  const { return vec2{this->z,this->y}; }
            vec3 xyz() const { return vec3{this->x, this->y, this->z}; }
            vec3 xzy() const { return vec3{this->x, this->z, this->y}; }
            vec3 yxz() const { return vec3{this->y, this->x, this->z}; }
            vec3 yzx() const { return vec3{this->y, this->z, this->x}; }
            vec3 zxy() const { return vec3{this->z, this->x, this->y}; }
            vec3 zyx() const { return vec3{this->z, this->y, this->x}; }
        };

        using int2 = vector<int,2>;
        using float2 = vector<float,2>;
        using double2 = vector<double,2>;

        using int3 = vector<int,3>;
        using float3 = vector<float,3>;
        using double3 = vector<double,3>;

        using int4 = vector<int,4>;
        using float4 = vector<float,4>;
        using double4 = vector<double,4>;

        using int2x2 = matrix<int,2,2>;
        using float2x2 = matrix<float,2,2>;
        using double2x2 = matrix<double,2,2>;

        using int3x3 = matrix<int,3,3>;
        using float3x3 = matrix<float,3,3>;
        using double3x3 = matrix<double,3,3>;

        using int4x4 = matrix<int,4,4>;
        using float4x4 = matrix<float,4,4>;
        using double4x4 = matrix<double,4,4>;

        template<typename S, size_t L, size_t C>
        std::ostream &operator<<(std::ostream &out, const matrix<S,L,C> &m)
        {
            int l;
            for (l = 0; l < L-1; ++l)
            {
                int c;
                out << "(";
                for (c = 0; c < C-1; ++c)
                    out << m(l,c) << ",";
                out << m(l,c) << ")" << std::endl;
            }
            int c;
            out << "(";
            for (c = 0; c < C-1; ++c)
                out << m(l,c) << ",";
            out << m(l,c) << ")";

            return out;
        }
    }
}

#endif
