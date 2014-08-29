#include "window.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdexcept>

namespace tiny { namespace rendering {

Window::Window(int width, int height, std::string title) : mHeight(height), mWidth(width), mIsOpen(true)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		throw std::runtime_error("could not initialize SDL");

	mSDLWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

	if (mSDLWindow == NULL)
		throw std::runtime_error("could not create window");

	mGLContext = SDL_GL_CreateContext(mSDLWindow);

	if (mGLContext == NULL)
		throw std::runtime_error("could not create GL context");

	glewExperimental = GL_TRUE; 
	if (GLEW_OK != glewInit())
		throw std::runtime_error("could not initiazlie GLEW");
}

Window::~Window() 
{
	SDL_GL_DeleteContext(mGLContext);
	SDL_DestroyWindow(mSDLWindow);
	SDL_Quit();
}

void Window::update()
{
	SDL_GL_SwapWindow(mSDLWindow);
}


}}
