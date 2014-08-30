#include <iostream>
#include <tiny/rendering/window.h>
#include <tiny/rendering/shaderprogram.h>
#include <tiny/rendering/shader.h>
#include <tiny/core/types.h>
#include <tiny/core/inputs.h>
#include <tiny/core/keys.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <vector>

#include <iostream>


using tiny::rendering::Window;
using tiny::rendering::ShaderProgram;
using tiny::rendering::Shader;
using tiny::core::vec3;
using tiny::core::Inputs;
using tiny::core::Key;
using tiny::core::MouseButton;

#define NUM_BUFFERS 1

struct Triangle
{
	Triangle()
	{
		vertices.push_back(vec3(-0.5, -0.5, 0));
		vertices.push_back(vec3( 0,   0.5,  0));
		vertices.push_back(vec3(0.5,-0.5, 0));

		glGenVertexArrays(1, &vertexArrayHandle);
		glBindVertexArray(vertexArrayHandle);

		glGenBuffers(NUM_BUFFERS, vertexArrayBufferHandles);
		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBufferHandles[0]);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 0,0);

		glBindVertexArray(0);
	}

	~Triangle()
	{
		glDeleteVertexArrays(1, &vertexArrayHandle);
	}

	void draw()
	{
		glBindVertexArray(vertexArrayHandle);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);

	}


	GLuint vertexArrayHandle;
	GLuint vertexArrayBufferHandles[NUM_BUFFERS];
	std::vector<vec3> vertices;
};

ShaderProgram flatShader()
{
	ShaderProgram result;
	result.add(Shader::fromFile(GL_VERTEX_SHADER,   "res/shaders/flat_vertex.glsl"));
	result.add(Shader::fromFile(GL_FRAGMENT_SHADER, "res/shaders/flat_fragment.glsl"));
	result.link();
	return result;
}

int main(int argc, char **argv)
{
	Window window(1080,768, "3D");
	ShaderProgram shaderProgram = flatShader();
	Inputs inputs;	
	Triangle triangle;

	while (window.isOpen())
	{
		inputs.refresh();

		if (inputs.isWindowCloseRequested())
			window.close();

		if (inputs.isKeyHeld(Key::KEY_LEFT_CMD) && inputs.isKeyPressed(Key::KEY_Z))
			window.close();

		glClearColor(0,0,0.5,1);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.use();
		triangle.draw();
		window.update();
	}

	return EXIT_SUCCESS;
}