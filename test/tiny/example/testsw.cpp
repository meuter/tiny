#include <iostream>
#include <tiny/core/window.h>
#include <tiny/core/inputs.h>
#include <tiny/core/types.h>
#include <tiny/rendering/sw/framebuffer.h>

using namespace tiny::core;
using namespace tiny::rendering::sw;

bool shouldQuit(const Inputs &inputs)
{
	if (inputs.isWindowCloseRequested())
		return true;

	if (inputs.isKeyHeld(Key::KEY_LEFT_CMD) && inputs.isKeyPressed(Key::KEY_Z))
		return true;

	return false;
}


void putpixel(byte *pixels, int x, int y, byte r, byte g, byte b)
{
	Framebuffer(pixels, 1900,1080).putpixel(x,y, vec3(0,1,0));
}

int main()
{
	Window window(1900,1080, "Software Rendering", Window::SOFTWARE);
	Framebuffer buffer(window);
	Inputs inputs;
	bool quit;


	do
	{
		inputs.refresh();
		window.clear();

	    for (int i = 0; i < 100; ++i)
			buffer.putpixel(50, i, vec3(0,1,0));

		window.swap();
	}
	while (!shouldQuit(inputs));

}