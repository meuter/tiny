#ifndef __TINY_RENDERING_GL_TEXTURE_H__
#define __TINY_RENDERING_GL_TEXTURE_H__

#include <GL/glew.h>
#include <string>

#include <tiny/core/types.h>

#include "handle.h"

namespace tiny { namespace rendering { namespace gl {

	class Texture
	{
		using byte = core::byte;
		using vec3 = core::vec3;
	public:	

		Texture();
		Texture(Texture &&other) = default;
		Texture(const vec3 &color);
		Texture(const std::string &filename);
		Texture(unsigned char *pixels, size_t width, size_t height);

		virtual ~Texture() = default;

		Texture &operator=(Texture &&other) = default;

		Texture &fromFile(const std::string &filename);
		Texture &fromPixels(byte *pixels, size_t width, size_t height);
		Texture &fromColor(const vec3 &color);

		void bind(GLuint slot) const;

		inline GLuint handle() const { return mHandle; }

	private:
		static void create(GLuint &handle);
		static void destroy(GLuint handle);

		Handle<GLuint,destroy> mHandle;
	};

}}}

#endif