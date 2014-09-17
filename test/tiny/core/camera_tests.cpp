#include <tiny/core/camera.h>
#include <tiny/core/types.h>
#include <gtest/gtest.h>


using namespace testing;
using namespace tiny::core;

TEST(Camera, has_a_default_constructor_where_the_projection_is_not_even_initialized)
{
	Camera camera;
}

TEST(Camera, has_a_constructor_that_takes_the_projection_as_input)
{
	mat4 expectedProjection = {
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};

	Camera camera(expectedProjection);

	EXPECT_EQ(expectedProjection, camera.projection());
}

TEST(Camera, is_transformable)
{
	Camera camera;
	Transformable *transformable = dynamic_cast<Transformable *>(&camera);

	EXPECT_NE(nullptr, transformable);
	EXPECT_EQ(vec3(0,0,0), camera.position());
	EXPECT_EQ(vec3(1,1,1), camera.scale());
	EXPECT_EQ(quat(0,0,0,1), camera.rotation());
}

TEST(Camera, is_aligned_with_the_original_coordinate_system_by_default)
{
	Camera camera;

	vec3 p(0,0,0);
	mat4 expectedView = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	vec3 transformedP = (vec4(p,1.0f) * camera.getView()).xyz();

	EXPECT_EQ(expectedView, camera.getView());
	EXPECT_EQ(p, transformedP);
}

TEST(Camera, can_be_used_to_change_coordinate_system)
{
	Camera camera;

	camera.moveTo(0,0,5);

	vec3 p(0,0,0);
	mat4 expectedView = {
		1.0f, 0.0f, 0.0f,  0.0f,
		0.0f, 1.0f, 0.0f,  0.0f,
		0.0f, 0.0f, 1.0f, -5.0f,
		0.0f, 0.0f, 0.0f,  1.0f
	};

	vec3 transformedP = (camera.getView() * vec4(p,1.0f).transposed()).transposed().xyz();

	EXPECT_EQ(expectedView, camera.getView());
	EXPECT_EQ(vec3(0,0,-5), transformedP);
}


TEST(Camera, has_a_look_at_function_that_can_be_used_to_point_the_camera_in_the_direction_of_a_point)
{
	Camera camera;

	camera.moveTo(0,0,5);
	camera.lookAt(0,0,0);

	vec3 p(0,0,0);
	mat4 expectedView = {
		-1.0f, 0.0f,  0.0f,  0.0f,
		 0.0f, 1.0f,  0.0f,  0.0f,
		 0.0f, 0.0f, -1.0f,  5.0f,
		 0.0f, 0.0f,  0.0f,  1.0f
	};

	vec3 transformedP = (camera.getView() * vec4(p,1.0f).transposed()).transposed().xyz();

	EXPECT_EQ(expectedView, camera.getView());
	EXPECT_EQ(vec3(0,0,5), transformedP);
}