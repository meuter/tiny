#include "engine.h"
#include "stopwatch.h"
#include "game.h"
#include <thread>


namespace tiny { namespace core {

Engine::Engine(Game &game) : mGame(game), mIsRunning(false)
{

}

Engine::~Engine()
{

}

void Engine::start()
{
	if (isRunning())
		return;

	mIsRunning = true;
	mGame.init();
	run();
}

bool Engine::isRunning() const
{
	return mIsRunning;
}

void Engine::stop()
{
	mIsRunning = false;
}

void Engine::run()
{
	const auto dt = sec(1.0/5000.0);
	auto stopwatch = Stopwatch();
	auto unprocessedTime = sec(0);

	while (isRunning())
	{
		unprocessedTime += stopwatch.lap();

		if (unprocessedTime < dt)
		{
			std::this_thread::sleep_for(msec(1));
			continue;
		}
			
		while (unprocessedTime >= dt)
		{
			mGame.getInputs().refresh();
			mGame.inputs();
			mGame.update(dt);
			unprocessedTime -= dt;			
		}

		mGame.getWindow().clear();
		mGame.render();				
		mGame.getWindow().swap();
	}
}


}}