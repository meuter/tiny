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

	void init() {}
	void update(Game &g, sec t, sec dt) {}

	void render()
	{
	    for (int i = 0; i < 100; ++i)
			mScreen.putpixel(50, i, vec3(0,1,0));
	}
private:
	Framebuffer &mScreen;
};


int main()
{
	Game game(Window(1900,1080, "Software Rendering", Window::SOFTWARE));
	Framebuffer screen(game.window());

	game.add(FPSCounter());
	game.add(WindowControl());
	game.add(MyComponent(screen));
	game.start();
	
	return EXIT_SUCCESS;
}