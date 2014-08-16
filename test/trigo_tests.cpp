#include <gtest/gtest.h>
#include <tiny/trigo.h>

using namespace tiny;

TEST(radian, can_be_defined)
{
    radian r{1.0};
}

TEST(radian, are_assignable)
{
    radian r{1.0};

    r = 1.5;
}


TEST(degree, can_be_defined)
{
    degree d{180};
}

TEST(radian, can_be_summed_and_compared)
{
    radian r{1.0}, s{2.5};

    EXPECT_LE(r, s+r);
}


TEST(radian, can_be_summed_and_compared_mixed_with_double)
{
    radian r{1.0}

    EXPECT_LE(r, r+1.5);
    EXPECT_LE(r, 1.5+r);
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
