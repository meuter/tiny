#include <tiny/matrix.h>
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

TEST(matrix, has_a_number_of_lines_and_columns_and_dimensions)
{
    matrix<int,5,2> m;
    auto d = m.dimensions();

    EXPECT_EQ(5, m.lines());
    EXPECT_EQ(2, m.columns());
    EXPECT_EQ(5, d.x);
    EXPECT_EQ(2, d.y);
}

TEST(int2x2, can_be_printed)
{
    int2x2 m = {
        { 1, 2 },
        { 3, 4 },
    };
    std::stringstream str;
    str << m;

    EXPECT_EQ("(1,2)\n(3,4)", str.str());
}


TEST(int2x2, can_be_summed)
{
    int2x2 m = {
        { 1, 2 },
        { 3, 4 }
    };

    int2x2 n = {
        { 4, 3 },
        { 2, 1 }
    };

    int2x2 r = m+n;

    EXPECT_EQ(5, r(0,0));
    EXPECT_EQ(5, r(1,0));
    EXPECT_EQ(5, r(0,1));
    EXPECT_EQ(5, r(1,1));
}

TEST(int2, can_be_modified)
{
    int2 v;

    v.x = 10;
    v.y = 20;
    EXPECT_EQ(10, v.x);
    EXPECT_EQ(20, v.y);
}

TEST(int2, can_be_summed)
{
    int2 u{1,2}, v{9,3}, w = u+v;

    EXPECT_EQ(10, w.x);
    EXPECT_EQ(5, w.y);
}

TEST(int2, has_a_dimension_of_2)
{
    int2 u;

    EXPECT_EQ(2, u.dimension());
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
