#ifndef __TINY_CORE_GAME_H__
#define __TINY_CORE_GAME_H__

#include <boost/noncopyable.hpp>

#include "engine.h"
#include "inputs.h"
#include "window.h"


namespace tiny { namespace core {

	class Game : public boost::noncopyable
	{
	public:
		Game(Window &&window) : mWindow(std::move(window)), mEngine(*this) {}
		Game(Game &&other) = default;
		virtual ~Game() {}

		Game &operator=(Game &&other) = default;

		virtual void init()                {}
		virtual void update(sec t, sec dt) {}
		virtual void render()              {}

		void start() { mEngine.start(); }
		void stop()  { mEngine.stop();  }

		inline Window &window()   { return mWindow; }
		inline Inputs &inputs()   { return mInputs; }

	private:
		Window mWindow;
		Inputs mInputs;
		Engine mEngine;
	};

}}

#endif