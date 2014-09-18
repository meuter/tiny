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

TEST(Transformable_objects, have_directions_that_adapt_to_rotation_around_y_axis)
{

	Transformable t;

	EXPECT_EQ(Transformable::X_AXIS, t.left());
	EXPECT_EQ(Transformable::Y_AXIS, t.up());
	EXPECT_EQ(Transformable::Z_AXIS, t.forward());

	t.rotate(Transformable::Y_AXIS, toRadian(90));

	EXPECT_EQ(-Transformable::Z_AXIS, t.left());
	EXPECT_EQ( Transformable::Y_AXIS, t.up());
	EXPECT_EQ( Transformable::X_AXIS, t.forward());

	t.rotate(Transformable::Y_AXIS, toRadian(90));

	EXPECT_EQ(-Transformable::X_AXIS, t.left());
	EXPECT_EQ( Transformable::Y_AXIS, t.up());
	EXPECT_EQ(-Transformable::Z_AXIS, t.forward());
}


TEST(Transformable_objects, have_directions_that_adapt_to_rotation_around_x_axis)
{
	Transformable t;

	EXPECT_EQ(Transformable::X_AXIS, t.left());
	EXPECT_EQ(Transformable::Y_AXIS, t.up());
	EXPECT_EQ(Transformable::Z_AXIS, t.forward());

	t.rotate(Transformable::X_AXIS, toRadian(90));

	EXPECT_EQ( Transformable::X_AXIS, t.left());
	EXPECT_EQ( Transformable::Z_AXIS, t.up());
	EXPECT_EQ(-Transformable::Y_AXIS, t.forward());

	t.rotate(Transformable::X_AXIS, toRadian(90));

	EXPECT_EQ( Transformable::X_AXIS, t.left());
	EXPECT_EQ(-Transformable::Y_AXIS, t.up());
	EXPECT_EQ(-Transformable::Z_AXIS, t.forward());
}

TEST(Transformable_objects, have_directions_that_adapt_to_rotation_around_z_axis)
{
	Transformable t;

	EXPECT_EQ(Transformable::X_AXIS, t.left());
	EXPECT_EQ(Transformable::Y_AXIS, t.up());
	EXPECT_EQ(Transformable::Z_AXIS, t.forward());

	t.rotate(Transformable::Z_AXIS, toRadian(90));

	EXPECT_EQ( Transformable::Y_AXIS, t.left());
	EXPECT_EQ(-Transformable::X_AXIS, t.up());
	EXPECT_EQ( Transformable::Z_AXIS, t.forward());

	t.rotate(Transformable::Z_AXIS, toRadian(90));

	EXPECT_EQ(-Transformable::X_AXIS, t.left());
	EXPECT_EQ(-Transformable::Y_AXIS, t.up());
	EXPECT_EQ( Transformable::Z_AXIS, t.forward());
}

TEST(Transformable_objects, have_a_translation_matrix_to_move_form_model_space_to_world_space)
{
	Transformable model;
	vec3 pInModelSpace(1,1,1), pInWorldSpace(2,3,4);

	model.moveTo(1,2,3);

	EXPECT_EQ(pInWorldSpace, (model.translationMatrix() * vec4(pInModelSpace,1)).xyz());
}

TEST(Transformable_objects, have_a_scaling_matrix_to_move_from_model_space_to_world_space)
{
	Transformable model;
	vec3 pInModelSpace(1,1,1), pInWorldSpace(1,2,3);
	vec3 qInModelSpace(0,0,0), qInWorldSpace(0,0,0);
	vec3 rInModelSpace(2,2,2), rInWorldSpace(2,4,6);

	model.scaleTo(1,2,3);

	EXPECT_EQ(pInWorldSpace, (model.scalingMatrix() * vec4(pInModelSpace,1)).xyz());
	EXPECT_EQ(qInWorldSpace, (model.scalingMatrix() * vec4(qInModelSpace,1)).xyz());
	EXPECT_EQ(rInWorldSpace, (model.scalingMatrix() * vec4(rInModelSpace,1)).xyz());
}

TEST(Transformable_objects, have_a_rotation_matrix_to_move_from_model_space_to_world_space__y_axis_only)
{
	Transformable model;
	vec3 pInModelSpace(2,0,0), pInWorldSpace(0,0,-2);
	vec3 qInModelSpace(0,2,0), qInWorldSpace(0,2,0);
	vec3 rInModelSpace(0,0,2), rInWorldSpace(2,0,0);

	model.rotateTo(Transformable::Y_AXIS, toRadian(90));
	EXPECT_EQ(pInWorldSpace, (model.rotationMatrix() * vec4(pInModelSpace,1)).xyz());
	EXPECT_EQ(qInWorldSpace, (model.rotationMatrix() * vec4(qInModelSpace,1)).xyz());
	EXPECT_EQ(rInWorldSpace, (model.rotationMatrix() * vec4(rInModelSpace,1)).xyz());
}


TEST(Transformable_objects, have_a_rotation_matrix_to_move_from_model_space_to_world_space__z_axis_only)
{
	Transformable model;
	vec3 pInModelSpace(2,0,0), pInWorldSpace(0,2,0);
	vec3 qInModelSpace(0,2,0), qInWorldSpace(-2,0,0);
	vec3 rInModelSpace(0,0,2), rInWorldSpace(0,0,2);

	model.rotateTo(Transformable::Z_AXIS, toRadian(90));
	EXPECT_EQ(pInWorldSpace, (model.rotationMatrix() * vec4(pInModelSpace,1)).xyz());
	EXPECT_EQ(qInWorldSpace, (model.rotationMatrix() * vec4(qInModelSpace,1)).xyz());
	EXPECT_EQ(rInWorldSpace, (model.rotationMatrix() * vec4(rInModelSpace,1)).xyz());
}

TEST(Transformable_objects, have_a_rotation_matrix_to_move_from_model_space_to_world_space__x_axis_only)
{
	Transformable model;
	vec3 pInModelSpace(2,0,0), pInWorldSpace(2,0,0);
	vec3 qInModelSpace(0,2,0), qInWorldSpace(0,0,2);
	vec3 rInModelSpace(0,0,2), rInWorldSpace(0,-2,0);

	model.rotateTo(Transformable::X_AXIS, toRadian(90));
	EXPECT_EQ(pInWorldSpace, (model.rotationMatrix() * vec4(pInModelSpace,1)).xyz());
	EXPECT_EQ(qInWorldSpace, (model.rotationMatrix() * vec4(qInModelSpace,1)).xyz());
	EXPECT_EQ(rInWorldSpace, (model.rotationMatrix() * vec4(rInModelSpace,1)).xyz());
}

