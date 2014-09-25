#include <gtest/gtest.h>
#include <tiny/math/quaternion.h>
#include <tiny/math/matrix.h>
#include <tiny/math/trigo.h>

using namespace tiny::math;

using quat = tiny::math::quaternion<float>;
using vec3 = float3;

const float3 xAxis(1,0,0), yAxis(0,1,0), zAxis(0,0,1);


TEST(quaternion, can_be_declared)
{
	quat q;
	quat r(1,2,3,4);

	vec3 axis(1,0,0);
	quat s(axis, toRadian(45));
}

TEST(quaternion, can_be_conjugated)
{
	quat q(1,2,3,4), expected(-1,-2,-3,4);

	EXPECT_EQ(expected, q.conjugate());
}

TEST(quaternion, can_be_multiplied_together)
{
	EXPECT_EQ(quat(0,0,0,8), quat(0,0,0,2) * quat(0,0,0,4));
}

TEST(quaternion, can_be_used_to_rotate_3d_vectors)
{
	EXPECT_EQ(-xAxis, quat(yAxis, toRadian(180)).rotate(xAxis));
	EXPECT_EQ(-3*xAxis, quat(yAxis, toRadian(180)).rotate(3*xAxis));
	EXPECT_EQ(yAxis, quat(zAxis, toRadian(90)).rotate(xAxis));
	EXPECT_EQ(-xAxis, quat(yAxis, toRadian(-90)).rotate(zAxis));
}

