#ifndef __TINY_CORE_FPS_COUNTER_H__
#define __TINY_CORE_FPS_COUNTER_H__

#include <iostream>
#include "types.h"
#include "engine.h"

namespace tiny { namespace core {

	template<class Game>
	class FPSCounter : public Game
	{

	public:

		void init(Engine &engine)
		{
			nFrames = 0;
			sinceLastFrame = sec(0);
			Game::init(engine);
		}

		void inputs(Engine &engine)
		{
			Game::inputs(engine);
		}

		void update(Engine &engine, sec dt)
		{
			sinceLastFrame += dt;
			if( sinceLastFrame > sec(1))
			{
				std::cout << nFrames << " FPS" << std::endl;
				nFrames = 0;
				sinceLastFrame = sec(0);
			}

			Game::update(engine, dt);
		}

		void render(Engine &engine)
		{
			Game::render(engine);
			nFrames++;
		}

	private:
		int nFrames;	
		sec sinceLastFrame;
	};

}}

#endif