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
                               boost::multiplicative2<matrix<S,L,C>, S> >
        {
        public:
            matrix()
            {
            }

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

            constexpr size_t lines()   const
            {
                return L;
            }

            constexpr size_t columns() const
            {
                return C;
            }

            constexpr matrix<S,1,2> dimensions() const
            {
                return matrix<S,1,2>{ { L, C } };
            }

            matrix<S,L,C> &operator+=(const matrix<S,L,C> &other)
            {
                for (size_t l = 0; l < lines(); ++l)
                    for (size_t c = 0; c < columns(); ++c)
                        (*this)(l,c) += other(l,c);
                return (*this);
            }

            matrix<S,L,C> &operator-=(const matrix<S,L,C> &other)
            {
                for (size_t l = 0; l < lines(); ++l)
                    for (size_t c = 0; c < columns(); ++c)
                        (*this)(l,c) -= other(l,c);
                return (*this);
            }

            matrix<S,L,C> &operator*=(const S &s)
            {
                for (size_t l = 0; l < lines(); ++l)
                    for (size_t c = 0; c < columns(); ++c)
                        (*this)(l,c) *= s;
                return (*this);
            }

            matrix<S,L,C> &operator/=(const S &s)
            {
                for (size_t l = 0; l < lines(); ++l)
                    for (size_t c = 0; c < columns(); ++c)
                        (*this)(l,c) /= s;
                return (*this);
            }

            template<size_t M>
            matrix<S,L,M> operator*(const matrix<S,C,M> &other) const
            {
                matrix<S,L,M> result;
                for (size_t i = 0; i < result.lines(); ++i)
                {
                    for (size_t j = 0; j < result.columns(); ++j)
                    {
                        result(i,j) = 0;
                        for (size_t k = 0; k < columns(); ++k)
                            result(i,j) += (*this)(i,k) * other(k, j);
                    }
                }
                return result;
            }

        };

        template<typename S, size_t N>
        class vector : public matrix<S,1,N>
        {
        public:
            vector()
            {
            }

            vector(const vector<S,N> &other) : matrix<S,1,N>{ other }
            {
            }

            vector(const matrix<S,1,N> &other) : matrix<S,1,N>{ other }
            {
            }

            vector(const std::initializer_list<S> &content) : matrix<S,1,N>{ content }
            {
            }

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

            auto length() const -> decltype(std::sqrt(S()))
            {
                S accumulator = 0;
                for (size_t i = 0; i < N; ++i)
                    accumulator += (*this)(i);
                return std::sqrt(accumulator);
            }

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
