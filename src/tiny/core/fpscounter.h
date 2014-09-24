#ifndef __TINY_CORE_FPS_COUNTER_H__
#define __TINY_CORE_FPS_COUNTER_H__

#include <iostream>
#include "types.h"

namespace tiny { namespace core {

	class FPSCounter 
	{
	public:
		FPSCounter() : nFrames(0), sinceLastFrame(0.0) {}

		void init()
		{
			nAverage = 0;	
			nSample = 0;
		}

		void update(sec t, sec dt)
		{
			sinceLastFrame += dt;
			if( sinceLastFrame > sec(1) )
			{
				nAverage = (nAverage*nSample + nFrames) / (nSample+1);
				++ nSample;

				std::cout << nFrames << " FPS (" << nAverage << " FPS avg.)" << std::endl;

				nFrames = 0;
				sinceLastFrame = sec(0);
			}
		}

		void render()
		{
			nFrames++;
		}

	private:
		int nFrames;
		int nAverage, nSample;
		sec sinceLastFrame;

	};

}}

#endif