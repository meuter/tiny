#include "window.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdexcept>

namespace tiny { namespace core {

Window::Window() : mHandle(NULL), mIsOpen(false)
{

}

Window::Window(int width, int height, std::string title) : mHandle(NULL), mHeight(height), mWidth(width), mIsOpen(true)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		throw std::runtime_error("could not initialize SDL");

	mHandle = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

	if (mHandle == NULL)
		throw std::runtime_error("could not create window");
}

Window::Window(Window &&other) : mHandle(other.mHandle), mHeight(other.mHeight), mWidth(other.mWidth), mIsOpen(other.mIsOpen)
{
	other.mHandle = NULL;
}

Window::~Window() 
{
	destroy();
}

Window &Window::operator=(Window &&other)
{
	destroy();

	mHandle = other.mHandle;
	mHeight    = other.mHeight;
	mWidth     = other.mWidth;

	other.mHandle = NULL;
	return (*this);
}


void Window::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::swap()
{
	SDL_GL_SwapWindow(mHandle);
}

void Window::destroy()
{
	if (mHandle != NULL)
	{
		SDL_DestroyWindow(mHandle);
		SDL_Quit();
	}	
}


}}
