#include "window.h"
#include <SDL2/SDL.h>
#include <GLEW/glew.h>

namespace tiny { namespace rendering {


Window::Window(int width, int height, std::string title) : mHeight(height), mWidth(width)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		throw std::runtime_error("could not initialize SDL");

	mSDLWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

	if (mSDLWindow == NULL)
		throw std::runtime_error("could not create window");

}

Window::~Window() 
{
	SDL_DestroyWindow(mSDLWindow);
	SDL_Quit();
}


}}
