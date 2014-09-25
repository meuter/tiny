#ifndef __TINY_RENDERING_SW_H__
#define __TINY_RENDERING_SW_H__

#include <tiny/core/window.h>
#include <tiny/core/types.h>

namespace tiny { namespace rendering {namespace sw {

	class Framebuffer
	{
		using byte = core::byte;
		using vec3 = core::vec3;
	public:	
		Framebuffer(const core::Window &window) : mPixels(window.pixels()), mWidth(window.width()), mHeight(window.height()) {}
		Framebuffer(byte *pixels, int width, int height) : mPixels(pixels), mWidth(width), mHeight(height) {}
		virtual ~Framebuffer() = default;

		inline void putpixel(int x, int y, vec3 color)
		{
		    byte *pixel = mPixels + 4 * (y * mHeight + x);

	        pixel[2] = (byte)(color.x * 255);
			pixel[1] = (byte)(color.y * 255);
			pixel[0] = (byte)(color.z * 255);
		}

	private:
		byte *mPixels;
		int mWidth, mHeight, mPitch;
	};

}}}

#endif