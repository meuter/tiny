#ifndef __TINY_CORE_STOPWATCH_H__
#define __TINY_CORE_STOPWATCH_H__

#include "types.h"
#include <chrono>

namespace tiny { namespace core {

	class Stopwatch
	{
		using high_res = std::chrono::high_resolution_clock;

	public:	
		Stopwatch() : mStart(high_res::now()) {}

		inline sec lap() 
		{
			auto newTime = high_res::now();
			auto result  = newTime - mStart;
			mStart = newTime;
			return result;
		}

	private:
		std::chrono::time_point<high_res> mStart;
	};

}}

#endif