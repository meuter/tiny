#include <cmath>
#include <tiny/units.h>
#include <boost/math/constants/constants.hpp>

namespace tiny
{
    const double PI = boost::math::constants::pi<double>();
    const double PI_2 = PI / 2.0;

    DEFINE_UNIT(radian, double);
    DEFINE_UNIT(degree, double);

    radian toRadian(const degree &angle)
    {
        return radian( (angle / 180.0) * PI );
    }

    degree toDegree(const radian &angle)
    {
        return degree( (angle / PI) * 180.0);
    }

    double cos(const radian &angle)
    {
        return std::cos(angle);
    }

    double cos(const degree &angle)
    {
        return std::cos(toRadian(angle));
    }

    double sin(const radian &angle)
    {
        return std::sin(angle);
    }

    double sin(const degree &angle)
    {
        return std::sin(toRadian(angle));
    }

    double tan(const radian &angle)
    {
        return std::tan(angle);
    }

    double tan(const degree &angle)
    {
        return std::tan(toRadian(angle));
    }


    double cotan(const radian &angle)
    {
        return std::tan(PI_2 - angle);
    }

    double cotan(const degree &angle)
    {
        return std::tan(PI_2 - toRadian(angle));
    }
}
