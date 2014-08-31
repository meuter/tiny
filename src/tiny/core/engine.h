#ifndef __TINY_CORE_ENGINE_H__
#define __TINY_CORE_ENGINE_H__

#include <functional>
#include "types.h"

namespace tiny { namespace core {

	class Game;

	class Engine
	{
	public:
		Engine(Game &game);
		Engine(Engine &&engine) = default;
		Engine(const Engine &other) = delete;
		virtual ~Engine();

		Engine &operator=(const Engine &other) = delete;

		void start();
		bool isRunning() { return mIsRunning; }
		void run();
		void stop();

	private:
		Game &mGame;
		bool mIsRunning;	
	};


}}

#endif