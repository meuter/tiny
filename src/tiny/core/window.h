#ifndef __TINY_GC_WINDOW_H__
#define __TINY_GC_WINDOW_H__

#include <string>
#include "types.h"

struct SDL_Window;
struct SDL_Surface;

namespace tiny { namespace core {

	class Window 
	{
	public:
		enum Backend : uint32_t 
		{
			NONE,
			OPENGL,
			SOFTWARE,
		};

		Window();
		Window(int width, int height, std::string title, Backend backend);
		Window(const Window &other) = delete;
		Window(Window &&window);
		virtual ~Window();

		Window &operator=(const Window &other) = delete;
		Window &operator=(Window &&other);

		inline int   width()  const { return mHeight; }
		inline int   height() const { return mWidth; }
		inline float aspect() const { return (float)mWidth/(float)mHeight; }		
		inline ivec2 center() const { return ivec2(mWidth/2, mHeight/2); }

		void clear();
		void swap();

		bool isOpen() { return mIsOpen; }
		void close()  { mIsOpen = false; }

		SDL_Window *handle() const { return mHandle; }

		byte *pixels() const;

	protected:
		void destroy();

	private:
		SDL_Window *mHandle;
		int mHeight, mWidth;
		int mIsOpen;
		Backend mBackend;
	};

}}

#endif
