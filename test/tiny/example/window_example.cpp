#include <iostream>
#include <tiny/rendering/window.h>
#include <tiny/rendering/shaderprogram.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>


using tiny::rendering::Window;
using tiny::rendering::ShaderProgram;


int main(int argc, char **argv)
{
	try {

		bool quit;
		Window window(1080,768, "3D");
		ShaderProgram shaderProgram("res/shaders/flat_vertex.glsl", "res/shaders/flat_fragment.glsl");

		shaderProgram.use();

		do
		{
			SDL_Event e;
			while(SDL_PollEvent(&e))
				quit = (e.type == SDL_QUIT);

			glClearColor(0,0,0,1);
			glClear(GL_COLOR_BUFFER_BIT);

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