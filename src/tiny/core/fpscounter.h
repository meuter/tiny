#ifndef __TINY_CORE_FPS_COUNTER_H__
#define __TINY_CORE_FPS_COUNTER_H__

#include <iostream>
#include "game.h"

namespace tiny { namespace core {

	class FPSCounter : public Game::Component
	{
	public:
		FPSCounter() : nFrames(0), mSinceLastFrame(0.0) {}

		void init()
		{
			nAverage = 0;	
			nSample  = 0;
			nFrames  = 0;
			mSinceLastFrame = sec{0.0};
		}

		void update(Game &game, sec t, sec dt)
		{
			mSinceLastFrame += dt;
			if( mSinceLastFrame > sec(1) )
			{
				nAverage = (nAverage*nSample + nFrames) / (nSample+1);
				++ nSample;

				std::cout << nFrames << " FPS (" << nAverage << " FPS avg.)" << std::endl;

				nFrames = 0;
				mSinceLastFrame = sec(0);
			}
		}

		void render()
		{
			nFrames++;
		}

	private:
		int nFrames;
		int nAverage, nSample;
		sec mSinceLastFrame;

	};

}}

#endif