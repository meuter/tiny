#ifndef __TINY_GC_WINDOW_H__
#define __TINY_GC_WINDOW_H__

#include <string>
#include <tiny/core/inputs.h>

struct SDL_Window;

namespace tiny { namespace rendering {

	class Window 
	{
	public:
		Window(int width, int height, std::string title);
		Window(const Window &other) = delete;
		Window(Window &&window);
		virtual ~Window();

		Window &operator=(const Window &other) = delete;

		inline int   width()  const { return mHeight; }
		inline int   height() const { return mWidth; }
		inline float apsect() const { return (float)mHeight/(float)mWidth; }		
		inline bool  isOpen() const { return mIsOpen; }
		inline void  close()        { mIsOpen = false; }

		void update();

	private:
		SDL_Window *mSDLWindow;
		void *mGLContext;
		int mHeight, mWidth;
		bool mIsOpen;
	};

}}

#endif
