#include <tiny/vector.hpp>
#include <sstream>
#include <gtest/gtest.h>

using namespace tiny;


TEST(float2, can_be_declared)
{
    float2 v = {1.0f,2.0f};

    EXPECT_EQ(sizeof(float)*2, sizeof(float2));
    EXPECT_EQ(1.0f, v.x);
    EXPECT_EQ(2.0f, v.y);
}

TEST(float2, can_be_printed)
{
    float2 v = {1.0f,2.0f};
    std::stringstream str;
    str << v;

    EXPECT_EQ("(1,2)", str.str());
}

TEST(int3x3, can_be_declared)
{
    int3x3 m;

    EXPECT_EQ(sizeof(int)*3*3, sizeof(int3x3));
    EXPECT_EQ(3, m.lines());
    EXPECT_EQ(3, m.columns());
}

TEST(matrix, can_be_declared_and_initialized_with_scalars)
{
    matrix<int,2,3> m = {
        { 1,2,3 },
        { 4,5,6 }
    };

    EXPECT_EQ(1, m(0,0));
    EXPECT_EQ(2, m(0,1));
    EXPECT_EQ(3, m(0,2));

    EXPECT_EQ(4, m(1,0));
    EXPECT_EQ(5, m(1,1));
    EXPECT_EQ(6, m(1,2));
}
