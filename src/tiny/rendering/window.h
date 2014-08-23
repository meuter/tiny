#ifndef __TINY_GC_WINDOW_H__
#define __TINY_GC_WINDOW_H__

#include <string>

struct SDL_Window;

namespace tiny {

 	namespace rendering {


 		class Window 
 		{
		public:
			Window(int width, int height, std::string title);
			virtual ~Window();

			inline int width() const  { return mHeight; }
			inline int height() const { return mWidth; }

		private:
 			SDL_Window *mSDLWindow;
 			int mHeight, mWidth;
 		};

	}
}

#endif
