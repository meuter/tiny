#ifndef __TINY_GC_WINDOW_H__
#define __TINY_GC_WINDOW_H__

#include <cstdint>
#include <SDL2/SDL.h>
#include <GLEW/glew.h>
#include <string.h>


 namespace tiny {

 	namespace gl {


 		class Window 
 		{
		public:
			Window(int width, int height, std::string title) : mHeight(height), mWidth(width)
			{
				if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
					throw std::runtime_error("could not initialize SDL");

				mSDLWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

				if (mSDLWindow == NULL)
					throw std::runtime_error("could not create window");

			}

			virtual ~Window() 
			{
				SDL_DestroyWindow(mSDLWindow);
				SDL_Quit();
			}

			int width() const 
			{
				return mHeight;
			}

			int height() const 
			{
				return mWidth;
			}

		private:
 			SDL_Window *mSDLWindow;
 			int mHeight, mWidth;
 		};

	}
}

#endif
