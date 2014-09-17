#include <tiny/core/camera.h>
#include <tiny/core/types.h>
#include <gtest/gtest.h>
#include <iomanip>
#include <cstring>


using namespace testing;
using namespace tiny::core;
using namespace tiny::math;

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

TEST(Camera, is_aligned_with_the_original_coordinate_system_by_default)
{
	Camera camera;

	vec3 p(0,0,0);
	vec3 transformedP = (camera.getViewMatrix() * vec4(p,1.0f)).xyz();

	EXPECT_EQ(vec3(1,0,0), camera.left());
	EXPECT_EQ(vec3(0,1,0), camera.up());
	EXPECT_EQ(vec3(0,0,1), camera.forward());
	EXPECT_EQ(p, transformedP);
}

TEST(Camera, has_a_vew_matrix_to_change_from_world_space_to_camera_space__move_z_only)
{
	Camera camera;
	camera.moveTo(0,0,5);

	vec3 p(0,0,0);
	vec3 transformedP = (camera.getViewMatrix() * vec4(p,1.0f)).xyz();

	EXPECT_EQ(vec3(1,0,0), camera.left());
	EXPECT_EQ(vec3(0,1,0), camera.up());
	EXPECT_EQ(vec3(0,0,1), camera.forward());
	EXPECT_EQ(vec3(0,0,-5), transformedP);
}

TEST(Camera, has_a_vew_matrix_to_change_from_world_space_to_camera_space__move_z_turn_around)
{
	Camera camera;
	camera.moveTo(0,0,5);
	camera.rotate(Transformable::Y_AXIS, toRadian(180));

	vec3 p(0,0,0);
	vec3 transformedP = (camera.getViewMatrix() * vec4(p,1.0f)).xyz();
	
	EXPECT_EQ(vec3(-1,0,0), camera.left());
	EXPECT_EQ(vec3(0,1,0), camera.up());
	EXPECT_EQ(vec3(0,0,-1), camera.forward());
	EXPECT_EQ(vec3(0,0,5), transformedP);
}

TEST(Camera, has_a_vew_matrix_to_change_from_world_space_to_camera_space__move_x_only)
{
	Camera camera;
	camera.moveTo(5,0,0);

	vec3 p(0,0,0);
	vec3 transformedP = (camera.getViewMatrix() * vec4(p,1.0f)).xyz();

	EXPECT_EQ(vec3(1,0,0), camera.left());
	EXPECT_EQ(vec3(0,1,0), camera.up());
	EXPECT_EQ(vec3(0,0,1), camera.forward());
	EXPECT_EQ(vec3(-5,0,0), transformedP);
}

TEST(Camera, has_a_vew_matrix_to_change_from_world_space_to_camera_space__move_x_and_turn_around)
{
	Camera camera;
	camera.moveTo(5,0,0);
	camera.rotate(Transformable::Y_AXIS,toRadian(-90));

	EXPECT_EQ(Transformable::Y_AXIS,  camera.up());
	EXPECT_EQ(Transformable::Z_AXIS,  camera.left());
	EXPECT_EQ(-Transformable::X_AXIS, camera.forward());

	vec3 p(0,0,0);
	vec3 transformedP = (camera.getViewMatrix() * vec4(p,1.0f)).xyz();

	EXPECT_EQ(vec3(0,0,5), transformedP);	
}

TEST(Camera, has_a_vew_matrix_to_change_from_world_space_to_camera_space__move_x_z_and_turn_to_origin)
{
	Camera camera;
	camera.moveTo(3,0,3);
	camera.rotate(Transformable::Y_AXIS,toRadian(-90-45));

	vec3 p(0,0,0);
	vec3 transformedP = (camera.getViewMatrix() * vec4(p,1.0f)).xyz();

	EXPECT_EQ(normalize(vec3(-1,0,-1)), camera.forward());
	EXPECT_EQ(Transformable::Y_AXIS, camera.up());
	EXPECT_EQ(normalize(vec3(-1, 0, 1)), camera.left());
	EXPECT_EQ(vec3(0,0,sqrt(18)), transformedP);	
}


TEST(Camera, has_a_vew_matrix_to_change_from_world_space_to_camera_space__move_y_only)
{
	Camera camera;
	camera.moveTo(0,5,0);

	vec3 p(0,0,0);
	vec3 transformedP = (camera.getViewMatrix() * vec4(p,1.0f)).xyz();

	EXPECT_EQ(vec3(1,0,0), camera.left());
	EXPECT_EQ(vec3(0,1,0), camera.up());
	EXPECT_EQ(vec3(0,0,1), camera.forward());
	EXPECT_EQ(vec3(0,-5,0), transformedP);
}

TEST(Camera, has_a_vew_matrix_to_change_from_world_space_to_camera_space__move_x_and_turn_to_origin)
{
	Camera camera;
	camera.moveTo(0,5,0);
	camera.rotate(Transformable::X_AXIS,toRadian(90));

	EXPECT_EQ(Transformable::Z_AXIS,  camera.up());
	EXPECT_EQ(Transformable::X_AXIS,  camera.left());
	EXPECT_EQ(-Transformable::Y_AXIS, camera.forward());

	vec3 p(0,0,0);
	vec3 transformedP = (camera.getViewMatrix() * vec4(p,1.0f)).xyz();

	EXPECT_EQ(vec3(0,0,5), transformedP);	
}

TEST(Camera, has_a_projection_matrix_to_change_from_camera_space_to_screen_space)
{
	Camera camera = Camera::withPerspective(toRadian(70), 800/600, 0.01f, 1000.0f);

	camera.moveTo(0,0,5);
	camera.rotateTo(Transformable::Y_AXIS, toRadian(180));

	vec4 pInWorldSpace(0,0,0,1);
	vec4 qInWorldSpace(0,0,-1,1);

	vec4 pInCameraSpace = camera.getViewMatrix() * pInWorldSpace;
	vec4 qInCameraSpace = camera.getViewMatrix() * qInWorldSpace;

	EXPECT_LT(pInCameraSpace.z, qInCameraSpace.z);

	vec4 pInScreenSpace = camera.projection() * pInCameraSpace;
	vec4 qInScreenSpace = camera.projection() * qInCameraSpace;

	std::cout << "pInCameraSpace" << pInCameraSpace << std::endl;
	std::cout << "pInScreenSpace" << pInScreenSpace << std::endl;
	std::cout << std::endl;
	std::cout << "qInCameraSpace" << qInCameraSpace << std::endl;
	std::cout << "qInScreenSpace" << qInScreenSpace << std::endl;

	EXPECT_LT(pInScreenSpace.z, qInScreenSpace.z);
	EXPECT_GT(pInScreenSpace.z, 0);
	EXPECT_GT(qInScreenSpace.z, 0);


}