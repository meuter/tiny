#include <tiny/math/matrix.h>
#include <sstream>
#include <gtest/gtest.h>

using namespace tiny::math;

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

TEST(int2, has_a_length)
{
    int2 u{0,1};

    EXPECT_EQ(1, u.length());
}


TEST(int2, can_be_added)
{
    int2 u{1,2}, v{4,9};
    int2 s = u+v;

    EXPECT_EQ(s.x, 1+4);
    EXPECT_EQ(s.y, 2+9);
}

TEST(int2, can_be_substracted)
{
    int2 u{1,2}, v{4,9};
    int2 d = v-u;

    EXPECT_EQ(d.x, 4-1);
    EXPECT_EQ(d.y, 9-2);
}


TEST(int2, can_be_multiplied_by_scalar)
{
    int2 u{1,2}, v{2*u}, t{u*3};

    EXPECT_EQ(v.x, 2);
    EXPECT_EQ(v.y, 4);

    EXPECT_EQ(t.x, 3);
    EXPECT_EQ(t.y, 6);
}


TEST(int2, can_be_devided_by_scalar)
{
    int2 u{2,4}, v{u/2};

    EXPECT_EQ(v.x, 1);
    EXPECT_EQ(v.y, 2);
}


TEST(float3, can_be_normalized)
{
    float3 v{6,4,2};
    auto l = v.length();

    EXPECT_EQ(&v, &v.normalize());
    EXPECT_FLOAT_EQ(v.x, 6/l);
    EXPECT_FLOAT_EQ(v.y, 4/l);
    EXPECT_FLOAT_EQ(v.z, 2/l);
}


TEST(int3, has_dot_product)
{
    int3 u{1,2,3}, v{4,5,6};
    int expected = 1*4+2*5+3*6;

    EXPECT_EQ(expected, u.dot(v));
    EXPECT_EQ(expected, v.dot(u));
    EXPECT_EQ(expected, u % v);
    EXPECT_EQ(expected, v % u);
}

TEST(matrices, can_be_multiplied)
{
    matrix<int, 2, 3> a = {
        { 1, 2, 3 },
        { 4, 5, 6 },
    };

    matrix<int, 3, 2> b = {
        { 7,  8  },
        { 9,  10 },
        { 11, 12 },
    };

    auto m = a*b;

    EXPECT_EQ(2, m.lines());
    EXPECT_EQ(2, m.columns());

    EXPECT_EQ(58, m(0,0));
    EXPECT_EQ(64, m(0,1));
    EXPECT_EQ(139, m(1,0));
    EXPECT_EQ(154, m(1,1));
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
