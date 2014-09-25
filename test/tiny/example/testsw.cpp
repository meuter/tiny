#include <iostream>
#include <tiny/core/window.h>
#include <tiny/core/inputs.h>
#include <tiny/core/types.h>
#include <tiny/core/game.h>
#include <tiny/core/fpscounter.h>

#include <tiny/rendering/sw/framebuffer.h>

using namespace tiny::core;
using namespace tiny::rendering::sw;

class MyGame : public Game
{
public:	
	MyGame(Window &&window) : Game(std::move(window)), mScreen(this->window()) {}

	void init()
	{
	
	}


	void update(sec t, sec dt)
	{
		if (shouldStop())
			stop();

		mFPSCounter.update(*this, t, dt);
	}

	void render()
	{
	    for (int i = 0; i < 100; ++i)
			mScreen.putpixel(50, i, vec3(0,1,0));

		mFPSCounter.render();
	}

	bool shouldStop()
	{
		if (inputs().isWindowCloseRequested())
			return true;

		if (inputs().isKeyHeld(Key::KEY_LEFT_CMD) && inputs().isKeyPressed(Key::KEY_Z))
			return true;

		return false;
	}

private:	
	Framebuffer mScreen;
	FPSCounter mFPSCounter;
};


int main()
{
	MyGame(Window(1900,1080, "Software Rendering", Window::SOFTWARE)).start();
	return EXIT_SUCCESS;
}