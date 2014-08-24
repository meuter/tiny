#include <iostream>
#include <tiny/rendering/window.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>


using tiny::rendering::Window;

int main(int argc, char **argv)
{
	bool quit;
	Window window(1080,768, "3D");

	do
	{
		SDL_Event e;
		while(SDL_PollEvent(&e))
			quit = (e.type == SDL_QUIT);

		glClearColor(0,0,0.8f,1);
		glClear(GL_COLOR_BUFFER_BIT);


		window.update();
	}
	while(!quit);
			

	return EXIT_SUCCESS;
}