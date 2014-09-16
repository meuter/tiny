#include "window.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdexcept>

namespace tiny { namespace core {

Window::Window() : mSDLWindow(NULL), mGLContext(NULL), mIsOpen(false)
{

}

Window::Window(int width, int height, std::string title) 
	: mSDLWindow(NULL), mGLContext(NULL), mHeight(height), mWidth(width), mIsOpen(true)
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

	glClearColor(0,0,0,1);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
}

Window::Window(Window &&other) 
	: mSDLWindow(other.mSDLWindow), mGLContext(other.mGLContext), mHeight(other.mHeight), mWidth(other.mWidth), mIsOpen(other.mIsOpen)
{
	other.mSDLWindow = NULL;
	other.mGLContext = NULL;
}

Window::~Window() 
{
	destroy();
}

Window &Window::operator=(Window &&other)
{
	destroy();

	mSDLWindow = other.mSDLWindow;
	mGLContext = other.mGLContext;
	mHeight    = other.mHeight;
	mWidth     = other.mWidth;

	other.mSDLWindow = NULL;
	other.mGLContext = NULL;
	return (*this);
}

void Window::vsync(bool onoff)
{
	SDL_GL_SetSwapInterval(onoff ? 1 : 0);
}

void Window::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::clear(float r, float g, float b, float a)
{
	glClearColor(r,g,b,a);
	clear();
}

void Window::swap()
{
	SDL_GL_SwapWindow(mSDLWindow);
}

void Window::destroy()
{
	if (mSDLWindow != NULL)
	{
		SDL_GL_DeleteContext(mGLContext);
		SDL_DestroyWindow(mSDLWindow);
		SDL_Quit();
	}	
}


}}
