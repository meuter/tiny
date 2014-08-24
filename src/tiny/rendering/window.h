#ifndef __TINY_GC_WINDOW_H__
#define __TINY_GC_WINDOW_H__

#include <string>
#include <SDL2/SDL.h>
#include <GL/glew.h>

namespace tiny {

 	namespace rendering {


 		class Window 
 		{
		public:
			Window(int width, int height, std::string title);
			virtual ~Window();

			inline int width() const  { return mHeight; }
			inline int height() const { return mWidth; }
			inline float apsect() const { return (float)mHeight/(float)mWidth; }

		private:
 			SDL_Window *mSDLWindow;
 			SDL_GLContext mGLContext;
 			int mHeight, mWidth;
 		};

	}
}

#endif
