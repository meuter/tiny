#ifndef __TINY_MATH_TRIGO_H__
#define __TINY_MATH_TRIGO_H__

#include <cmath>
#include <tiny/units.h>
#include <boost/math/constants/constants.hpp>

namespace tiny
{
    namespace math
    {

        const double PI = boost::math::constants::pi<double>();
        const double PI_2 = PI / 2.0;

        DEFINE_UNIT(radian, double);
        DEFINE_UNIT(degree, double);

        inline radian toRadian(const double &angle)
        {
            return radian( (angle / 180.0) * PI );
        }

        inline degree toDegree(const double &angle)
        {
            return degree( (angle / PI) * 180.0);
        }

        inline double cos(const radian &angle)
        {
            return std::cos(angle);
        }

        inline double cos(const degree &angle)
        {
            return std::cos(toRadian(angle));
        }

        inline double sin(const radian &angle)
        {
            return std::sin(angle);
        }

        inline double sin(const degree &angle)
        {
            return std::sin(toRadian(angle));
        }

        inline double tan(const radian &angle)
        {
            return std::tan(angle);
        }

        inline double tan(const degree &angle)
        {
            return std::tan(toRadian(angle));
        }


        inline double cotan(const radian &angle)
        {
            return std::tan(PI_2 - angle);
        }

        inline double cotan(const degree &angle)
        {
            return std::tan(PI_2 - toRadian(angle));
        }
    }
}

#endif