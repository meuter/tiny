#ifndef __TINY_CORE_GAME_H__
#define __TINY_CORE_GAME_H__

#include "inputs.h"
#include "window.h"

namespace tiny { namespace core {

	class Game 
	{
	public:	
		class Component 
		{
		public:
			Component() = default;
			Component(Component &&component) = default;
			Component(const Component &component) = default;

			virtual ~Component() = default;

			Component &operator=(Component &&component) = default;
			Component &operator=(const Component &component) = default;

			virtual void init() {}
			virtual void update(Game &game, sec t, sec dt) {}
			virtual void render() {}
		};

	public:	
		Game(Window &&window) : mWindow(std::move(window)){}
		Game(Game &&other) = default;
		virtual ~Game() {}

		Game &operator=(Game &&other) = default;

		virtual void init()                {}
		virtual void update(sec t, sec dt) {}
		virtual void render()              {}

		void start();
		void stop();

		inline Window &window()   { return mWindow; }
		inline Inputs &inputs()   { return mInputs; }

	protected:
		void run();

	private:
		Window mWindow;
		Inputs mInputs;
		bool mIsRunning;
	};

}}

#endif