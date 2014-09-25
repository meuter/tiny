#ifndef __TINY_CORE_CONTROLS_H__
#define __TINY_CORE_CONTROLS_H__


#include "transformable.h"
#include "game.h"

namespace tiny { namespace core {

	class WindowControl 
	{
	public:	
		WindowControl()  {}
		virtual ~WindowControl() = default;

		void init()   {}
		void render() {}
		void update(Game &game, sec t, sec dt)
		{
			if (game.inputs().isWindowCloseRequested())
				game.stop();

			if (game.inputs().isKeyHeld(Key::KEY_LEFT_CMD) && game.inputs().isKeyPressed(Key::KEY_Z))
				game.stop();
		}
	};

	class KeyboardControl 
	{
	public:
		KeyboardControl(Transformable &controlled) : mControlled(controlled) {}
		virtual ~KeyboardControl() = default;

		void init()   {}
		void render() {}
		void update(Game &game, sec t, sec dt)
		{
			float amount = dt.count() * 10;

			if (game.inputs().isKeyHeld(Key::KEY_UP))
				mControlled.move(mControlled.forward(), amount);
			if (game.inputs().isKeyHeld(Key::KEY_DOWN))
				mControlled.move(mControlled.backward(), amount);
			if (game.inputs().isKeyHeld(Key::KEY_LEFT))
				mControlled.move(mControlled.left(), amount);
			if (game.inputs().isKeyHeld(Key::KEY_RIGHT))
				mControlled.move(mControlled.right(), amount);
			if (game.inputs().isKeyHeld(Key::KEY_PAGEUP))
				mControlled.move(mControlled.up(), amount);
			if (game.inputs().isKeyHeld(Key::KEY_PAGEDOWN))
				mControlled.move(mControlled.down(), amount);	
		}

	private:
		Transformable &mControlled;
	};

	class MouseControl 
	{
	public:
		MouseControl(Transformable &controlled) : mControlled(controlled) {}
		virtual ~MouseControl() = default;

		void init()   {}
		void render() {}
		void update(Game &game, sec t, sec dt)
		{
			const float sensitivity = 0.005f;

			if (game.inputs().isMouseReleased(MouseButton::MIDDLE))
			{
				game.inputs().showMouseCursor(true);		
			}
			else if (game.inputs().isMousePressed(MouseButton::MIDDLE))
			{
				game.inputs().showMouseCursor(false);
				game.inputs().setMousePosition(game.window().center());
			}
			else if (game.inputs().isMouseHeld(MouseButton::MIDDLE))
			{
		 		auto dpos = game.window().center() - game.inputs().getMousePosition();

				if (dpos.x != 0)
					mControlled.rotate(mControlled.up(), rad{dpos.x * sensitivity});

				if (dpos.y != 0)
					mControlled.rotate(mControlled.right(), rad{dpos.y * sensitivity});

				if (dpos.x != 0 || dpos.y != 0)
					game.inputs().setMousePosition(game.window().center());
			}
		}

	private:
		Transformable &mControlled;
	};


}}

#endif