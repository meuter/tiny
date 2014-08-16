#include <gtest/gtest.h>
#include <tiny/trigo.h>

using tiny::radian;

TEST(radian, can_be_defined)
{
    radian r{1.0};
}

TEST(radian, can_be_summed)
{
    radian r{1.0}, s{2.5};

    r<=s;
}

TEST(cos, works_on_radians)
{
    radian angle{2};
}
