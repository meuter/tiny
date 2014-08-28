#include <iostream>
#include <tiny/rendering/window.h>
#include <tiny/rendering/shaderprogram.h>
#include <tiny/core/types.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <vector>


using tiny::rendering::Window;
using tiny::rendering::ShaderProgram;
using tiny::core::vec3;

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



int main(int argc, char **argv)
{
	try {

		bool quit;
		Window window(1080,768, "3D");
		ShaderProgram shaderProgram("res/shaders/flat_vertex.glsl", "res/shaders/flat_fragment.glsl");
		Triangle triangle;

		do
		{
			SDL_Event e;
			while(SDL_PollEvent(&e))
				quit = (e.type == SDL_QUIT);

			glClearColor(0,0,0.5,1);
			glClear(GL_COLOR_BUFFER_BIT);

			shaderProgram.use();
			triangle.draw();

			window.update();
		}
		while(!quit);
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}
			

	return EXIT_SUCCESS;
}