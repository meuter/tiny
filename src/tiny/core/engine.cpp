#include "engine.h"
#include "clock.h"
#include "game.h"
#include <thread>


namespace tiny { namespace core {

Engine::Engine(rendering::Window && window) : mWindow(std::move(window))
{

}

Engine::~Engine()
{

}

void Engine::play(Game &&game)
{
	auto clock = Clock();
	auto unprocessedTime = sec(0);

	game.init(*this);

	while (mWindow.isOpen())
	{
		unprocessedTime += clock.lap();

		if (unprocessedTime < delta())
		{
			std::this_thread::sleep_for(msec(1));
			continue;
		}
			
		while (unprocessedTime >= delta())
		{
			mInputs.refresh();
			game.inputs(*this);
			game.update(*this);
			unprocessedTime -= delta();			
		}

		mWindow.clear();
		game.render(*this);
		mWindow.swapBuffer();
	}
}

}}