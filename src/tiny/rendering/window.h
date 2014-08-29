#ifndef __TINY_GC_WINDOW_H__
#define __TINY_GC_WINDOW_H__

#include <string>
#include <tiny/math/matrix.h>

struct SDL_Window;

namespace tiny { namespace rendering {


	class Window 
	{
	public:		
		using MousePosition = math::int2;

		enum MouseButton
		{
			MOUSE_LEFT_BUTTON   = 1,
			MOUSE_MIDDLE_BUTTON = 2,
			MOUSE_RIGHT_BUTTON  = 3,
			MOUSE_WHEEL_UP      = 4,
			MOUSE_WHEEL_DOWN    = 5
		};

		enum KeyCode
		{
			KEY_RETURN = 40,
			KEY_ESCAPE = 41,
			KEY_BACKSPACE = 42,
			KEY_TAB = 43,
			KEY_SPACE = 44,

			KEY_LEFT_CTRL = 224,
			KEY_LEFT_SHIFT = 225,
			KEY_LEFT_ALT = 226,			
			KEY_LEFT_CMD = 227,


			KEY_RIGHT_CTRL = 228,
			KEY_RIGHT_SHIFT = 229,
			KEY_RIGHT_ALT = 230,
			KEY_RIGHT_CMD = 231,

			KEY_A = 4,
			KEY_B = 5,
			KEY_C = 6,
			KEY_D = 7,
			KEY_E = 8,
			KEY_F = 9,
			KEY_G = 10,
			KEY_H = 11,
			KEY_I = 12,
			KEY_J = 13,
			KEY_K = 14,
			KEY_L = 15,
			KEY_M = 16,
			KEY_N = 17,
			KEY_O = 18,
			KEY_P = 19,
			KEY_Q = 20,
			KEY_R = 21,
			KEY_S = 22,
			KEY_T = 23,
			KEY_U = 24,
			KEY_V = 25,
			KEY_W = 26,
			KEY_X = 27,
			KEY_Y = 28,
			KEY_Z = 29,

		};

		enum 
		{
			NUMBER_OF_MOUSE_BUTTONS  = 256,
			NUMBER_OF_KEY_CODES      = 512,
		};

	public:
		Window(int width, int height, std::string title);
		virtual ~Window();

		inline int   width()  const { return mHeight; }
		inline int   height() const { return mWidth; }
		inline float apsect() const { return (float)mHeight/(float)mWidth; }		
		inline bool  isOpen() const { return mIsOpen; }
		inline void  close()        { mIsOpen = false; }

		inline bool  isKeyDown(KeyCode keyCode)      const { return mKeyDown[keyCode]; }
		inline bool  isKeyReleased(KeyCode keyCode)  const { return mKeyReleased[keyCode]; }
		inline bool  isKeyPressed(KeyCode keyCode)   const { return mKeyPressed[keyCode]; }

		inline bool  isMouseUp(MouseButton button)   const { return mMouseUp[button]; }
		inline bool  isMouseDown(MouseButton button) const { return mMouseDown[button]; }

		inline MousePosition getMousePosition()      const { return mMousePosition; }

		void update();

	private:
		SDL_Window *mSDLWindow;
		void *mGLContext;
		int mHeight, mWidth;

		std::vector<bool> mMouseUp, mMouseDown, mKeyDown, mKeyPressed, mKeyReleased;
		MousePosition mMousePosition;
		bool mIsOpen;
	};

}}

#endif
