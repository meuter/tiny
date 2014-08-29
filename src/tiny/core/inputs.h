#ifndef __TINY_CORE_INPUTS_H__
#define __TINY_CORE_INPUTS_H__

#include <vector>
#include "keys.h"

namespace tiny { namespace core {

	class Inputs
	{		
	public:
		Inputs();

		void update();
		inline bool isWindowCloseRequested() const { return mWindowCloseRequested; }

		inline bool isKeyPressed(Key key)  const { return mKeyPressed[static_cast<int>(key)]; }
		inline bool isKeyReleased(Key key) const { return mKeyReleased[static_cast<int>(key)]; }
		inline bool isKeyHeld(Key key)     const { return mKeyHeld[static_cast<int>(key)]; }

	private:	
		bool mWindowCloseRequested;
		std::vector<bool> mKeyPressed, mKeyReleased, mKeyHeld;
	};

}}

#endif