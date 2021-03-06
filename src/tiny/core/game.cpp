#include "game.h"
#include "stopwatch.h"
#include <thread>

namespace tiny { namespace core {

	void Game::start()
	{
		mIsRunning = true;
		run();
	}

	void Game::stop()
	{
		mIsRunning = false;
	}

	void Game::run()
	{
		const auto dt = sec(1.0/5000.0);
		auto t = sec(0);
		auto stopwatch = Stopwatch();
		auto unprocessedTime = sec(0);

		mRoot.init();

		while (mIsRunning)
		{
			unprocessedTime += stopwatch.lap();
			
			if (unprocessedTime < dt)
			{
				std::this_thread::sleep_for(msec(20));
				continue;
			}
				
			while (unprocessedTime >= dt)
			{
				mInputs.refresh();
				mRoot.update(*this, t, dt);
				unprocessedTime -= dt;			
				t += dt;
			}

			mWindow.clear();
			mRoot.render();				
			mWindow.swap();
		}
	}


}}