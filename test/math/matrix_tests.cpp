#include <tiny/math/matrix.h>
#include <sstream>
#include <gtest/gtest.h>

using namespace tiny::math;

TEST(float2, can_be_declared)
{
    float2 u;
    float2 v{1.0f,2.0f};

    EXPECT_EQ(sizeof(float)*2, sizeof(float2));
    EXPECT_EQ(1.0f, v.x);
    EXPECT_EQ(2.0f, v.y);
}

TEST(int2, can_be_printed)
{
    int2 v{1,2};
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

TEST(int3, can_be_compared_for_equality)
{
    int3 u(1,2,3), v(1,2,3), t(1,2,5);

    EXPECT_EQ(u, v);
    EXPECT_EQ(u, u);
    EXPECT_NE(t, u);
}

TEST(matrix, has_a_number_of_lines)
{
    matrix<int,5,2> m;

    EXPECT_EQ(5, m.lines());
    EXPECT_EQ(2, m.columns());
}

TEST(int2x2, can_be_printed)
{
    int2x2 m {
        1, 2,
        3, 4,
    };
    std::stringstream str;
    str << m;

    EXPECT_EQ("(1,2)\n(3,4)", str.str());
}


TEST(int2x2, can_be_summed)
{
    int2x2 m { 1, 2, 3, 4 };
    int2x2 n { 4, 3, 2, 1 };
    int2x2 r { 5, 5, 5, 5 };

    EXPECT_EQ(r, m+n);
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
    int2 u(1,2), v(9,3), w = u+v;

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
    int2 u(0,1);

    EXPECT_EQ(1, u.length());
}


TEST(int2, can_be_added)
{
    int2 u(1,2), v(4,9);
    int2 s = u+v;

    EXPECT_EQ(s.x, 1+4);
    EXPECT_EQ(s.y, 2+9);
}

TEST(int2, can_be_substracted)
{
    int2 u(1,2), v(4,9);
    int2 d = v-u;

    EXPECT_EQ(d.x, 4-1);
    EXPECT_EQ(d.y, 9-2);
}


TEST(int2, can_be_multiplied_by_scalar)
{
    int2 u(1,2), v(2*u), t(u*3);

    EXPECT_EQ(v.x, 2);
    EXPECT_EQ(v.y, 4);

    EXPECT_EQ(t.x, 3);
    EXPECT_EQ(t.y, 6);
}


TEST(int2, can_be_devided_by_scalar)
{
    int2 u(2,4), v(u/2);

    EXPECT_EQ(v.x, 1);
    EXPECT_EQ(v.y, 2);
}


TEST(float3, can_be_normalized)
{
    float3 v(6,4,2);
    auto l = v.length();

    EXPECT_EQ(&v, &v.normalize());
    EXPECT_FLOAT_EQ(v.x, 6/l);
    EXPECT_FLOAT_EQ(v.y, 4/l);
    EXPECT_FLOAT_EQ(v.z, 2/l);
}

TEST(general_sized_vector, has_dot_product)
{
    vector<int,5> u{1,2,3,4,10}, v{5,6,7,8,20};
    int expected = 1*5+2*6+3*7+4*8+10*20;

    EXPECT_EQ(expected, u % v);
    EXPECT_EQ(expected, v % u);
}

TEST(matrices, can_be_multiplied)
{
    matrix<int, 2, 3> a {
        1, 2, 3,
        4, 5, 6,
    };

    matrix<int, 3, 2> b {
        7,  8 ,
        9,  10,
        11, 12,
    };

    matrix<int, 2,2> expected {
        58, 64,
        139, 154,
    };

    auto actual = a*b;

    EXPECT_EQ(2, actual.lines());
    EXPECT_EQ(2, actual.columns());
    EXPECT_EQ(expected, actual);
}

TEST(int2, supports_swizzling)
{
    int2 v(1,2);

    EXPECT_EQ(int2(1,2), v.xy());
    EXPECT_EQ(int2(2,1), v.yx());
}

TEST(int3, supports_swizzling)
{
    int3 v(1,2,3);

    EXPECT_EQ(int2(1,2), v.xy());
    EXPECT_EQ(int2(1,3), v.xz());
    EXPECT_EQ(int2(2,1), v.yx());
    EXPECT_EQ(int2(2,3), v.yz());
    EXPECT_EQ(int2(3,1), v.zx());
    EXPECT_EQ(int2(3,2), v.zy());

    EXPECT_EQ(int3(1,2,3), v.xyz());
    EXPECT_EQ(int3(1,3,2), v.xzy());
    EXPECT_EQ(int3(2,1,3), v.yxz());
    EXPECT_EQ(int3(2,3,1), v.yzx());
    EXPECT_EQ(int3(3,1,2), v.zxy());
    EXPECT_EQ(int3(3,2,1), v.zyx());
}

TEST(int4, supports_swizzling)
{
    int4 v(1,2,3,4);

    EXPECT_EQ(int2(1,2), v.xy());
    EXPECT_EQ(int2(1,3), v.xz());
    EXPECT_EQ(int2(2,1), v.yx());
    EXPECT_EQ(int2(2,3), v.yz());
    EXPECT_EQ(int2(3,1), v.zx());
    EXPECT_EQ(int2(3,2), v.zy());

    EXPECT_EQ(int3(1,2,3), v.xyz());
    EXPECT_EQ(int3(1,3,2), v.xzy());
    EXPECT_EQ(int3(2,1,3), v.yxz());
    EXPECT_EQ(int3(2,3,1), v.yzx());

    EXPECT_EQ(int3(3,1,2), v.zxy());
    EXPECT_EQ(int3(3,2,1), v.zyx());
}

TEST(byte4, has_size_4)
{
    EXPECT_EQ(sizeof(byte), 1);
    EXPECT_EQ(sizeof(byte4), 4);
}


TEST(double3, can_be_compared)
{
    double3 v{1,2,3}, u{2,3,4};

    EXPECT_LE(v,v);
    EXPECT_GE(v,v);
    EXPECT_LE(v,u);
    EXPECT_LT(v,u);
    EXPECT_GE(u,v);
    EXPECT_GT(u,v);
}


TEST(int3, has_cross_product)
{
    int3 i(1,0,0), j(0,1,0), k(0,0,1);
    int4 l(1,0,0,0);

    EXPECT_EQ(i, j ^ k);
    EXPECT_EQ(j, k ^ i);
    EXPECT_EQ(k, i ^ j);

    int2x2 x {1,2,3,4};
}

TEST(float4, can_be_homogenized)
{
    float4 v(4,6,8,2), u = v;
    float4 expected(2,3,4,1);

    u.homogenize();

    EXPECT_EQ(expected, v.homogenized());
    EXPECT_EQ(expected, u);

}

TEST(matrix, can_be_declared_and_initialized_with_scalars)
{
    matrix<int,2,3> m {
        1,2,3,
        4,5,6
    };

    EXPECT_EQ(1, m(0,0));
    EXPECT_EQ(2, m(0,1));
    EXPECT_EQ(3, m(0,2));

    EXPECT_EQ(4, m(1,0));
    EXPECT_EQ(5, m(1,1));
    EXPECT_EQ(6, m(1,2));
}

TEST(matrix, can_be_transposed)
{
    matrix<int,2,3> m {
        1,2,3,
        4,5,6
    };
    auto orig = m;

    matrix<int,3,2> expected {
        1,4,
        2,5,
        3,6,
    };

    EXPECT_EQ(expected, m.transposed());
    EXPECT_EQ(orig, m);
}

TEST(square_matrix, can_be_transposed_in_place)
{
    int2x2 m{
        1,2,
        3,4
    };

    int2x2 expected {
        1,3,
        2,4
    };

    EXPECT_EQ(expected, m.transpose());
    EXPECT_EQ(expected, m);
}

TEST(float2x2, has_an_identify)
{
    float2x2 i {
        1.0f, 0.0f,
        0.0f, 1.0f
    };
}

TEST(vectors, can_be_scaled)
{
    int2 v{1,2}, u{3,3};
    int2 r{3,6}, orig = v;

    EXPECT_EQ(r, v.scaled(u));
    EXPECT_EQ(orig, v);
    EXPECT_EQ(r, v.scale(u));
    EXPECT_EQ(r, v);
}

TEST(square_matrixes, can_be_multiplied_together)
{
    int2x2 m {
        1,2,
        3,4
    };
    int2x2 n = m;
    n*=m;

    int2x2 expected {
        7,  10,
        15, 22,
    };

    matrix<int,2,3> o {
        1,2,3,
        4,5,6
    };

    EXPECT_EQ(expected, m*m);
    EXPECT_EQ(expected, n);
}
