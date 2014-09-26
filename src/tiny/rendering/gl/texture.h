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
	public:	

		Texture() = default;
		Texture(Texture &&other) = default;
		Texture(const std::string &filename);
		Texture(unsigned char *pixels, size_t width, size_t height);

		virtual ~Texture() = default;

		Texture &operator=(Texture &&other) = default;

		Texture &fromFile(const std::string &filename);

		void load(byte *pixels, size_t width, size_t height);
		void bind(GLuint slot) const;

	private:
		static void destroy(GLuint handle);

		Handle<GLuint,destroy> mHandle;
	};

}}}

#endif