#include <gtest/gtest.h>
#include <tiny/trigo.h>

using namespace tiny;

TEST(radian, can_be_defined)
{
    radian r{1.0};
}

TEST(degree, can_be_defined)
{
    degree d{180};
}

TEST(radian, can_be_summed)
{
    radian r{1.0}, s{2.5};

    r<=s;
}

TEST(radian, can_be_converted_to_radian)
{
    EXPECT_DOUBLE_EQ(180.0, toDegree(radian{PI}));
}

TEST(degree, can_be_converted_to_degree)
{
    EXPECT_DOUBLE_EQ(PI, toRadian(degree{180.0}));
}


TEST(trigo, works_on_radians_and_degrees)
{
    radian angleInRadian{PI/3};
    degree angleInDegree = toDegree(angleInRadian);

    EXPECT_DOUBLE_EQ(cos(angleInRadian), cos(angleInDegree));
    EXPECT_DOUBLE_EQ(sin(angleInRadian), sin(angleInDegree));
    EXPECT_DOUBLE_EQ(tan(angleInRadian), tan(angleInDegree));
    EXPECT_DOUBLE_EQ(cotan(angleInRadian), cotan(angleInDegree));
}
