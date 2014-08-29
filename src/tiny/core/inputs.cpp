#include "inputs.h"
#include <SDL2/SDL.h>


namespace tiny { namespace core {

Inputs::Inputs() 
	: mWindowCloseRequested(false),
	  mKeyPressed(static_cast<int>(Key::KEY_MAX), false), mKeyReleased(static_cast<int>(Key::KEY_MAX), false), mKeyHeld(static_cast<int>(Key::KEY_MAX), false)
{
}


void Inputs::update()
{
	mKeyPressed.assign(static_cast<int>(Key::KEY_MAX),false);
	mKeyReleased.assign(static_cast<int>(Key::KEY_MAX),false);

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
		default:
			break;
		}
	}
}

}}
