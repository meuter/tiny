#include "engine.h"
#include "clock.h"
#include "game.h"
#include <thread>


namespace tiny { namespace core {

Engine::Engine(rendering::Window && window) : mWindow(std::move(window)), mIsRunning(false) 
{

}

Engine::~Engine()
{

}

void Engine::start(Game &game)
{
	if (mIsRunning)
		return;

	mIsRunning = true;
	game.init(*this);
}  

void Engine::stop()
{
	if (!mIsRunning)
		return;

	mIsRunning = false;
}

void Engine::inputs(Game &game)
{
	mInputs.refresh();
	game.inputs(*this);
}

void Engine::render(Game &game)
{
	mWindow.clear();
	game.render(*this);
	mWindow.swapBuffer();
}


void Engine::play(Game &&game)
{
	auto clock = Clock();
	auto unprocessedTime = sec(0);

	start(game);

	while (mIsRunning)
	{
		unprocessedTime += clock.lap();

		if (unprocessedTime < dt())
		{
			std::this_thread::sleep_for(usec(100));
			continue;
		}
			
		while (unprocessedTime >= dt())
		{
			inputs(game);
			game.update(*this);
			unprocessedTime -= dt();			
		}

		render(game);
	}
}

}}