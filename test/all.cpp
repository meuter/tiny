#include <tiny/vector.hpp>
#include <sstream>
#include <gtest/gtest.h>

using namespace tiny;

TEST(float2, can_be_declared)
{
    float2 v{1.0f,2.0f};

    EXPECT_EQ(sizeof(float)*2, sizeof(float2));
    EXPECT_EQ(1.0f, v.x);
    EXPECT_EQ(2.0f, v.y);
}

TEST(float2, can_be_printed)
{
    float2 v{1.0f,2.0f};
    std::stringstream str;
    str << v;

    EXPECT_EQ("(1,2)", str.str());
}

TEST(int3x3, can_be_declared)
{
    int3x3 m;

    EXPECT_EQ(sizeof(int)*3*3, sizeof(int3x3));
}
