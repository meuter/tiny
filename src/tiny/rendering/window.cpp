#include "window.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdexcept>

namespace tiny { namespace rendering {

Window::Window(int width, int height, std::string title) 
	: mHeight(height), mWidth(width), 
	  mMouseUp(NUMBER_OF_MOUSE_BUTTONS, false), mMouseDown(NUMBER_OF_MOUSE_BUTTONS, false),
	  mKeyDown(NUMBER_OF_KEY_CODES, false), mKeyPressed(NUMBER_OF_KEY_CODES, false), mKeyReleased(NUMBER_OF_KEY_CODES, false),
	  mMousePosition(0,0), mIsOpen(true)
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

	mMouseUp.assign(NUMBER_OF_MOUSE_BUTTONS, false);
	mMouseDown.assign(NUMBER_OF_MOUSE_BUTTONS, false);
	mKeyPressed.assign(NUMBER_OF_KEY_CODES,false);
	mKeyReleased.assign(NUMBER_OF_KEY_CODES,false);

	SDL_Event event;	
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			mIsOpen = false;
			break;
		case SDL_MOUSEMOTION:
			mMousePosition.x = event.motion.x;
			mMousePosition.y = event.motion.y;
			break;
		case SDL_KEYUP:
			mKeyReleased[event.key.keysym.scancode] = true;
			mKeyDown[event.key.keysym.scancode] = false;
			break;
		case SDL_KEYDOWN:
			mKeyPressed[event.key.keysym.scancode] = true;
			mKeyDown[event.key.keysym.scancode] = true;
			break;
		case SDL_MOUSEBUTTONUP:
			mMouseUp[event.button.button] = true;
			break;
		case SDL_MOUSEBUTTONDOWN:
			mMouseDown[event.button.button] = true;
			break;
		default:
			break;
		}
	}
}


}}
