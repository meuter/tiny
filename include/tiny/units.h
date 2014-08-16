#include <boost/config.hpp>
#include <boost/operators.hpp>

#define DEFINE_UNIT(unit, T)                                                        \
    class unit : private boost::ordered_euclidean_ring_operators<unit >,            \
                         boost::ordered_euclidean_ring_operators<unit, T >          \
    {                                                                               \
    public:                                                                         \
        explicit unit(const T value) : mValue(value) {};                            \
        unit() {};                                                                  \
        unit(const unit &u) : mValue(u.mValue) {}                                   \
        operator T&() { return mValue; }                                            \
        operator const T&() const { return mValue; }                                \
        bool operator==(const unit& rhs) const { return mValue == rhs.mValue; }     \
        bool operator<(const unit& rhs)  const { return mValue < rhs.mValue; };     \
        unit& operator+=(const unit& rhs) { mValue += rhs.mValue; return *this; }   \
        unit& operator-=(const unit& rhs) { mValue -= rhs.mValue; return *this; }   \
        unit& operator*=(const unit& rhs) { mValue *= rhs.mValue; return *this; }   \
        unit& operator/=(const unit& rhs) { mValue /= rhs.mValue; return *this; }   \
        bool operator==(const T &rhs) const { return mValue == rhs; }               \
        bool operator<(const T &rhs)  const { return mValue < rhs; }                \
        unit& operator+=(const T &rhs) { mValue += rhs; return *this; }             \
        unit& operator-=(const T &rhs) { mValue -= rhs; return *this; }             \
        unit& operator*=(const T &rhs) { mValue *= rhs; return *this; }             \
        unit& operator/=(const T &rhs) { mValue /= rhs; return *this; }             \
    private:                                                                        \
        T mValue;                                                                   \
    }
