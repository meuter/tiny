#include <iostream>
#include <tiny/rendering/window.h>

using tiny::gl::Window;

int main(int argc, char **argv)
{
	bool quit;
	Window window(800,600, "3D");

	do
	{
		SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			if(e.type == SDL_QUIT)
				quit = true;
		}		
	}
	while(!quit);
			

	return EXIT_SUCCESS;
}