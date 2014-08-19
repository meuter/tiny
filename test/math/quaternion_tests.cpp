#include <gtest/gtest.h>
#include <tiny/math/quaternion.h>
#include <tiny/math/trigo.h>

using namespace tiny::math;

TEST(quaternion, can_be_declared)
{
	quaternion q;
	quaternion r(1,2,3,4);

	float3 axis(1,0,0);
	quaternion s(axis, toRadian(45));
}

TEST(quaternion, can_be_conjugated)
{
	quaternion q(1,2,3,4), expected(-1,-2,-3,4);

	EXPECT_EQ(expected, q.conjugate());
}

TEST(quaternion, can_be_multiplied_together)
{
	EXPECT_EQ(quaternion(0,0,0,8), quaternion(0,0,0,2) * quaternion(0,0,0,4));
}

TEST(quaternion, can_be_used_to_rotate_3d_vectors)
{
	float3 xAxis(1,0,0), yAxis(0,1,0), zAxis(0,0,1);

	EXPECT_EQ(-xAxis, quaternion(yAxis, toRadian(180)).rotate(xAxis));
	EXPECT_EQ(-3*xAxis, quaternion(yAxis, toRadian(180)).rotate(3*xAxis));
	EXPECT_EQ(yAxis, quaternion(zAxis, toRadian(90)).rotate(xAxis));
	EXPECT_EQ(-xAxis, quaternion(yAxis, toRadian(-90)).rotate(zAxis));
}
