#ifndef __TINY_CORE_GAME_H__
#define __TINY_CORE_GAME_H__

#include <vector>

#include "inputs.h"
#include "window.h"
#include "types.h"
#include "entity.h"

namespace tiny { namespace core {

	class Game 
	{
	public:	
		Game(Window &&window) : mWindow(std::move(window)){}
		Game(Game &&other) = default;
		virtual ~Game() {}

		Game &operator=(Game &&other) = default;

		void start();
		void stop();

		inline Window &window()   { return mWindow; }
		inline Inputs &inputs()   { return mInputs; }
		inline Entity &root()     { return mRoot; }

		inline const Window &window() const { return mWindow; }
		inline const Inputs &inputs() const { return mInputs; }
		inline const Entity &root()   const { return mRoot; }

	protected:
		void run();

	private:
		Entity mRoot;
		Window mWindow;
		Inputs mInputs;
		bool mIsRunning;
	};

}}

#endif