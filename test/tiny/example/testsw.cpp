#include <iostream>
#include <tiny/core/window.h>
#include <tiny/core/inputs.h>
#include <tiny/core/types.h>
#include <tiny/core/game.h>
#include <tiny/core/fpscounter.h>
#include <tiny/core/controls.h>

#include <tiny/rendering/sw/framebuffer.h>

using namespace tiny::core;
using namespace tiny::rendering::sw;

class MyComponent 
{
public:	
	MyComponent(Framebuffer &screen) : mScreen(screen) {}

	void init()   {}
	void render()
	{
	    for (int i = 0; i < 100; ++i)
			mScreen.putpixel(50, i, vec3(0,1,0));
	}

	void update(Game &g, sec t, sec dt) {}

private:
	Framebuffer &mScreen;
};

class MyGame : public Game
{
public:	
	MyGame(Window &&window) : Game(std::move(window)), mScreen(this->window())
	{
		root().add(FPSCounter());
		root().add(WindowControl());
		root().add(MyComponent(mScreen));
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