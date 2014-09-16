#ifndef __TINY_CORE_ENGINE_H__
#define __TINY_CORE_ENGINE_H__

#include <functional>
#include "types.h"
#include <boost/noncopyable.hpp>

namespace tiny { namespace core {

	class Game;

	class Engine : public boost::noncopyable
	{
	public:
		Engine(Game &game);
		Engine(Engine &&engine) = default;
		virtual ~Engine();

		Engine &operator=(Engine &&other) = default;

		void start();
		void stop();
		
		bool isRunning() const;

	protected:
		void run();

	private:
		Game &mGame;
		bool mIsRunning;	
	};


}}

#endif