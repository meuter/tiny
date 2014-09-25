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
		Game(Window &&window) : mWindow(std::move(window)), mIsRunning(false) {}
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

		template<typename T>
		void add(T &&t) { mRoot.add(std::move(t)); }

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