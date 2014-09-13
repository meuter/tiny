#include "inputs.h"
#include <SDL2/SDL.h>


namespace tiny { namespace core {

Inputs::Inputs() 
	: mWindowCloseRequested(false),
	  mKeyPressed(static_cast<int>(Key::KEY_MAX), false),
	  mKeyReleased(static_cast<int>(Key::KEY_MAX), false),
	  mKeyHeld(static_cast<int>(Key::KEY_MAX), false),
	  mMousePressed(static_cast<int>(MouseButton::MAX), false),
	  mMouseReleased(static_cast<int>(MouseButton::MAX), false),
	  mMouseHeld(static_cast<int>(MouseButton::MAX), false),
	  mMousePosition(0,0), mMouseWheel(0,0)
{
}

Inputs::~Inputs()
{	
}

void Inputs::refresh()
{
	mKeyPressed.assign(static_cast<int>(Key::KEY_MAX),false);
	mKeyReleased.assign(static_cast<int>(Key::KEY_MAX),false);
	mMousePressed.assign(static_cast<int>(MouseButton::MAX), false);
	mMouseReleased.assign(static_cast<int>(MouseButton::MAX), false);

	SDL_Event event;	
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			mWindowCloseRequested = true;
			break;
		case SDL_KEYUP:
			mKeyReleased[event.key.keysym.scancode] = true;
			mKeyHeld[event.key.keysym.scancode] = false;
			break;
		case SDL_KEYDOWN:
			mKeyPressed[event.key.keysym.scancode] = true;
			mKeyHeld[event.key.keysym.scancode] = true;
			break;
		case SDL_MOUSEBUTTONUP:
			mMouseReleased[event.button.button] = true;
			mMouseHeld[event.button.button] = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			mMousePressed[event.button.button] = true;
			mMouseHeld[event.button.button] = true;
			break;
		case SDL_MOUSEWHEEL:
			mMouseWheel = ivec2(event.wheel.x, event.wheel.y);
			break;
		case SDL_MOUSEMOTION:
			mMousePosition = ivec2(event.motion.x, event.motion.y);
		default:
			break;
		}
	}
}

void Inputs::showMouseCursor(bool visible) const
{
	SDL_ShowCursor(visible);
}

void Inputs::setMousePosition(ivec2 position) const 
{
	SDL_WarpMouseInWindow(NULL, position.x, position.y);
}


}}
