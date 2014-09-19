#include <gtest/gtest.h>
#include <tiny/core/window.h>
#include <tiny/rendering/gl/shader.h>
#include <GL/glew.h>

using namespace tiny::rendering;
using namespace tiny::rendering::gl;
using namespace tiny::core;
using namespace testing;

class ShaderTest : public Test
{
public:	
	ShaderTest() : window(10,10,"test") {}
	Window window;
};


TEST_F(ShaderTest, throw_runtime_error_if_file_does_not_exist)
{
	Shader shader(GL_VERTEX_SHADER);

	EXPECT_THROW(shader.loadFile("file_that_does_not_exist.glsl"), std::runtime_error);
}

TEST_F(ShaderTest, does_not_throw_if_file_exisits_and_is_empty)
{
	Shader shader(GL_VERTEX_SHADER);

	EXPECT_NO_THROW(shader.loadFile("test/tiny/rendering/test_shader.glsl"));
}

TEST(Window, can_be_moved_around)
{
	auto window = Window(10,10,"test");
}