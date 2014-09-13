#ifndef __TINY_CORE_INPUTS_H__
#define __TINY_CORE_INPUTS_H__

#include <vector>
#include "keys.h"
#include "mousebuttons.h"
#include "types.h"

namespace tiny { namespace core {

	class Inputs
	{		
	public:
		Inputs();
		Inputs(const Inputs &other) = delete;
		Inputs(Inputs &&other) = default;
		virtual ~Inputs();

		Inputs &operator=(const Inputs &other) = delete;

		void refresh();		
		
		inline bool isWindowCloseRequested() const { return mWindowCloseRequested; }

		inline bool isKeyPressed(Key key)  const { return mKeyPressed[static_cast<int>(key)]; }
		inline bool isKeyReleased(Key key) const { return mKeyReleased[static_cast<int>(key)]; }
		inline bool isKeyHeld(Key key)     const { return mKeyHeld[static_cast<int>(key)]; }

		inline bool isMousePressed(MouseButton button)  const { return mMousePressed[static_cast<int>(button)]; }
		inline bool isMouseReleased(MouseButton button) const { return mMouseReleased[static_cast<int>(button)]; }
		inline bool isMouseHeld(MouseButton button)     const { return mMouseHeld[static_cast<int>(button)]; }

		ivec2 getMousePosition() const { return mMousePosition; }
		ivec2 getMouseWheel()    const { return mMouseWheel; }

		void setMousePosition(ivec2 position) const;
		void showMouseCursor(bool visible) const;

	private:	
		bool mWindowCloseRequested;
		std::vector<bool> mKeyPressed, mKeyReleased, mKeyHeld;
		std::vector<bool> mMousePressed, mMouseReleased, mMouseHeld;
		ivec2 mMousePosition, mMouseWheel;
	};

}}

#endif