#include <gtest/gtest.h>
#include <GL/glew.h>
#include <tiny/rendering/shader.h>
#include <tiny/rendering/window.h>

using namespace tiny::rendering;
using namespace testing;

class ShaderTest : public Test
{
public:	
	ShaderTest() : window(10,10,"test") {}
	Window window;
};


TEST_F(ShaderTest, throw_runtime_error_if_file_does_not_exist)
{
	EXPECT_THROW(Shader(GL_VERTEX_SHADER, "file_that_does_not_exist.glsl"), std::runtime_error);
}

TEST_F(ShaderTest, does_not_throw_if_file_exisits_and_is_empty)
{
	Shader(GL_VERTEX_SHADER, "test/tiny/rendering/test_shader.glsl");
}