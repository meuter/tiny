#include <iostream>
#include <tiny/rendering/window.h>
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
	}
	while(!quit);
			

	return EXIT_SUCCESS;
}