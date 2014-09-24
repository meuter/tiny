#include "window.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdexcept>

namespace tiny { namespace core {

Window::Window() : mHandle(NULL), mIsOpen(false), mBackend(NONE)
{

}

Window::Window(int width, int height, std::string title, Backend backend) : mHandle(NULL), mHeight(height), mWidth(width), mIsOpen(true), mBackend(backend)
{
	uint32_t flags = mBackend == OPENGL ? SDL_WINDOW_OPENGL : 0;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		throw std::runtime_error("could not initialize SDL");

	mHandle = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);

	if (mHandle == NULL)
		throw std::runtime_error("could not create window");
}

Window::Window(Window &&other) : mHandle(other.mHandle), mHeight(other.mHeight), mWidth(other.mWidth), mIsOpen(other.mIsOpen), mBackend(other.mBackend)
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

	mHandle   = other.mHandle;
	mHeight   = other.mHeight;
	mWidth    = other.mWidth;
	mBackend  = other.mBackend;

	other.mHandle = NULL;
	return (*this);
}

byte *Window::pixels() const
{
	byte *pixels = (byte *)SDL_GetWindowSurface(mHandle)->pixels;

	if (pixels == NULL)
		throw std::runtime_error("could not get pixels");

	return pixels;
}

void Window::clear()
{
	if (mBackend == OPENGL)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		SDL_Surface *surface = SDL_GetWindowSurface(mHandle);
		SDL_FillRect(surface, NULL, 0x00000000);
	}
}

void Window::swap()
{
	if (mBackend == OPENGL)
		SDL_GL_SwapWindow(mHandle);
	else
		SDL_UpdateWindowSurface(mHandle);
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
