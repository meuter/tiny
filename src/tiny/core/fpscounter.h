#ifndef __TINY_CORE_FPS_COUNTER_H__
#define __TINY_CORE_FPS_COUNTER_H__

#include <iostream>
#include "types.h"

namespace tiny { namespace core {

	class FPSCounter 
	{
	public:
		FPSCounter() : nFrames(0), sinceLastFrame(0.0) {}

		void update(sec dt)
		{
			sinceLastFrame += dt;
			if( sinceLastFrame > sec(1))
			{
				std::cout << nFrames << " FPS" << std::endl;
				nFrames = 0;
				sinceLastFrame = sec(0);
			}
		}

		void newFrame()
		{
			nFrames++;
		}

	private:
		int nFrames;	
		sec sinceLastFrame;
	};

}}

#endif