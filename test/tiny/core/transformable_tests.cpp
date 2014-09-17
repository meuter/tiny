#include <gtest/gtest.h>
#include <tiny/core/transformable.h>
#include <tiny/math/trigo.h>

using namespace tiny::core;
using namespace tiny::math;
using namespace testing;

TEST(Transformable_objects, have_a_position_rotation_and_scaling)
{
	Transformable t;

	EXPECT_EQ(vec3(0,0,0),   t.position());
	EXPECT_EQ(vec3(1,1,1),   t.scaling());
	EXPECT_EQ(quat(0,0,0,1), t.rotation());
}

TEST(Transformable_objects, have_a_left_and_right_direction_along_the_x_axis)
{
	Transformable t;	

	EXPECT_EQ( Transformable::X_AXIS, t.left());
	EXPECT_EQ(-Transformable::X_AXIS, t.right());
}

TEST(Transformable_objects, have_a_up_and_down_direction_along_the_y_axis)
{
	Transformable t;	

	EXPECT_EQ( Transformable::Y_AXIS, t.up());
	EXPECT_EQ(-Transformable::Y_AXIS, t.down());
}

TEST(Transformable_objects, have_a_forward_and_backward_direction_along_the_z_axis)
{
	Transformable t;	

	EXPECT_EQ( Transformable::Z_AXIS, t.forward());
	EXPECT_EQ(-Transformable::Z_AXIS, t.backward());
}

TEST(Transformable_objects, can_be_moved)
{
	Transformable t;

	EXPECT_EQ(vec3(0,0,0), t.position());
	t.moveTo(1,1,1);
	EXPECT_EQ(vec3(1,1,1), t.position());
	t.moveTo(vec3(5,5,5));
	EXPECT_EQ(vec3(5,5,5), t.position());
	t.move(vec3(-1,-1,-1), 3);
	EXPECT_EQ(vec3(2,2,2), t.position());
}

TEST(Transformable_objects, can_be_rotated)
{
	Transformable t;
	quat rotationX(Transformable::X_AXIS, toRadian(90));
	quat rotationY(Transformable::Y_AXIS, toRadian(180));


	EXPECT_EQ(quat(0,0,0,1), t.rotation());
	t.rotateTo(rotationX);
	EXPECT_EQ(normalize(rotationX), t.rotation());
	t.rotateTo(Transformable::Y_AXIS, toRadian(180));
	EXPECT_EQ(normalize(rotationY), t.rotation());
	t.rotate(quat(Transformable::X_AXIS, toRadian(90)));
	EXPECT_EQ(normalize(rotationX * rotationY), t.rotation());
	t.rotate(Transformable::X_AXIS, toRadian(-90));
	EXPECT_EQ(normalize(rotationY), t.rotation());
}

TEST(Transformable_objects, can_be_scaled)
{
	Transformable t;

	EXPECT_EQ(vec3(1,1,1), t.scaling());
	t.scaleTo(vec3(2,3,4));
	EXPECT_EQ(vec3(2,3,4), t.scaling());
	t.scaleTo(-2,-5,-9);
	EXPECT_EQ(vec3(-2,-5,-9), t.scaling());
	t.scaleTo(2,3,4);
	t.scale(4,2,3);
	EXPECT_EQ(vec3(8,6,12), t.scaling());
	t.scale(vec3(1.0f/4.0f, 1.0f/2.0f, 1.0f/3.0f));
	EXPECT_EQ(vec3(2,3,4), t.scaling());


}