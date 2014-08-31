#ifndef __TINY_CORE_GAME_H__
#define __TINY_CORE_GAME_H__

#include "engine.h"
#include "inputs.h"
#include "window.h"

namespace tiny { namespace core {

	class Game
	{
	public:
		Game(Window &&window) : mWindow(std::move(window)), mEngine(*this) {}
		Game(const Game &other) = delete;
		Game(Game &&other) = default;
		virtual ~Game() {}

		Game &operator=(const Game &other) = delete;
		Game &operator=(Game &&other) = default;

		virtual void init()         {}
		virtual void inputs()       {}
		virtual void update(sec dt) {}
		virtual void render()       {}

		void start() { mEngine.start(); }
		void stop()  { mEngine.stop();  }

		inline Window &getWindow() { return mWindow; }
		inline Inputs &getInputs() { return mInputs; }

	private:
		Window mWindow;
		Inputs mInputs;
		Engine mEngine;
	};

}}

#endif