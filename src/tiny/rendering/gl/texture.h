#ifndef __TINY_RENDERING_GL_TEXTURE_H__
#define __TINY_RENDERING_GL_TEXTURE_H__

#include <GL/glew.h>
#include <string>
#include "handle.h"

namespace tiny { namespace rendering { namespace gl {

	class Texture
	{
	public:	
		static Texture fromFile(const std::string &filename);

		Texture() = default;
		Texture(Texture &&other) = default;
		Texture(unsigned char *pixels, size_t width, size_t height);

		virtual ~Texture() = default;

		Texture &operator=(Texture &&other) = default;

		void bind(GLuint slot) const;

	private:
		static void destroy(GLuint handle);

		Handle<GLuint,destroy> mHandle;
	};

}}}

#endif