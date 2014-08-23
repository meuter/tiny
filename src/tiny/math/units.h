#ifndef __TINY_MATH_UNIT_H__
#define __TINY_MATH_UNIT_H__

#include <boost/config.hpp>
#include <boost/operators.hpp>

#define DEFINE_UNIT(unit, T)                                                    \
    class unit : private boost::ordered_euclidean_ring_operators<unit>,         \
                         boost::ordered_euclidean_ring_operators<unit, T >      \
    {                                                                           \
    public:                                                                     \
        explicit unit(const T value) : mValue(value) {};                        \
        unit() {};                                                              \
        unit(const unit &u) : mValue(u.mValue) {}                               \
        operator T&() { return mValue; }                                        \
        operator const T&() const { return mValue; }                            \
        bool operator==(const unit& r) const { return mValue == r.mValue; }     \
        bool operator<(const unit& r)  const { return mValue < r.mValue; };     \
        unit& operator=(const unit& r) { mValue = r.mValue; return *this; }     \
        unit& operator+=(const unit& r) { mValue += r.mValue; return *this; }   \
        unit& operator-=(const unit& r) { mValue -= r.mValue; return *this; }   \
        unit& operator*=(const unit& r) { mValue *= r.mValue; return *this; }   \
        unit& operator/=(const unit& r) { mValue /= r.mValue; return *this; }   \
        bool operator==(const T &r) const { return mValue == r; }               \
        bool operator<(const T &r)  const { return mValue < r; }                \
        unit& operator=(const T &r) { mValue = r; return *this; }               \
        unit& operator+=(const T &r) { mValue += r; return *this; }             \
        unit& operator-=(const T &r) { mValue -= r; return *this; }             \
        unit& operator*=(const T &r) { mValue *= r; return *this; }             \
        unit& operator/=(const T &r) { mValue /= r; return *this; }             \
    private:                                                                    \
        T mValue;                                                               \
    }

#endif