#ifndef __TINY_GC_WINDOW_H__
#define __TINY_GC_WINDOW_H__

#include <string>
#include "types.h"

struct SDL_Window;

namespace tiny { namespace core {

	class Window 
	{
	public:
		Window();
		Window(int width, int height, std::string title);
		Window(const Window &other) = delete;
		Window(Window &&window);
		virtual ~Window();

		Window &operator=(const Window &other) = delete;
		Window &operator=(Window &&other);

		inline int   width()  const { return mHeight; }
		inline int   height() const { return mWidth; }
		inline float aspect() const { return (float)mWidth/(float)mHeight; }		
		inline ivec2 center() const { return ivec2(mWidth/2, mHeight/2); }

		void vsync(bool onoff);
		void clear();
		void clear(float r, float g, float b, float a);
		void swap();

		bool isOpen() { return mIsOpen; }
		void close()  { mIsOpen = false; }

	protected:
		void destroy();

	private:
		SDL_Window *mSDLWindow;
		void *mGLContext;
		int mHeight, mWidth;
		int mIsOpen;
	};

}}

#endif
