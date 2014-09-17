#include <tiny/core/camera.h>
#include <tiny/core/types.h>
#include <gtest/gtest.h>
#include <iomanip>
#include <cstring>


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
}

// TEST(Camera, is_aligned_with_the_original_coordinate_system_by_default)
// {
// 	Camera camera;

// 	vec3 p(0,0,0);
// 	vec3 transformedP = (camera.getView() * vec4(p,1.0f)).xyz();

// 	EXPECT_EQ(vec3(1,0,0), camera.right());
// 	EXPECT_EQ(vec3(0,1,0), camera.up());
// 	EXPECT_EQ(vec3(0,0,1), camera.forward());
// 	EXPECT_EQ(p, transformedP);
// }

// TEST(Camera, can_be_used_to_change_coordinate_system)
// {
// 	Camera camera;
// 	camera.moveTo(0,0,5);

// 	vec3 p(0,0,0);
// 	vec3 transformedP = (camera.getView() * vec4(p,1.0f)).xyz();

// 	EXPECT_EQ(vec3(1,0,0), camera.right());
// 	EXPECT_EQ(vec3(0,1,0), camera.up());
// 	EXPECT_EQ(vec3(0,0,1), camera.forward());
// 	EXPECT_EQ(vec3(0,0,-5), transformedP);
// }


// TEST(Camera, look_at_z_axis)
// {
// 	Camera camera;
// 	camera.moveTo(0,0,5);
// 	camera.lookAt(0,0,0);

// 	vec3 p(0,0,0);
// 	vec3 transformedP = (camera.getView() * vec4(p,1.0f)).xyz();
	
// 	EXPECT_EQ(vec3(1,0,0), camera.right());
// 	EXPECT_EQ(vec3(0,1,0), camera.up());
// 	EXPECT_EQ(vec3(0,0,1), camera.forward());
// 	EXPECT_EQ(vec3(0,0,5), transformedP);
// }

// TEST(Camera, look_at_x_axis)
// {
// 	Camera camera;
// 	camera.moveTo(5,0,0);
// 	camera.lookAt(0,0,0);

// 	std::cout << camera.right() << std::endl;
// 	std::cout << camera.up() << std::endl;
// 	std::cout << camera.forward() << std::endl;

// 	EXPECT_EQ(vec3(0,1,0), camera.up());
// 	EXPECT_EQ(vec3(0,0,-1), camera.right());
// 	EXPECT_EQ(vec3(-1,0,0), camera.forward());

// 	vec3 p(0,0,0);
// 	vec3 transformedP = (camera.getView() * vec4(p,1.0f)).xyz();

// 	EXPECT_EQ(vec3(0,0,-5), transformedP);	
// }

// TEST(Camera, look_at_xz_axis)
// {
// 	Camera camera;
// 	camera.moveTo(3,0,3);
// 	camera.lookAt(0,0,0);

// 	vec3 p(0,0,0);
// 	vec3 transformedP = (camera.getView() * vec4(p,1.0f)).xyz();

// 	EXPECT_EQ(vec3(0,0,sqrt(18)), transformedP);	
// }