#ifndef __TINY_RENDERING_TEXTURE_H__
#define __TINY_RENDERING_TEXTURE_H__

#include <GL/glew.h>
#include <string>
#include <boost/noncopyable.hpp>

namespace tiny { namespace rendering { namespace gl {

	class Texture : boost::noncopyable
	{
	public:	
		static Texture fromFile(const std::string &filename);

		Texture();
		Texture(Texture &&other);

		virtual ~Texture();

		Texture &operator=(Texture &&other);

		void   load(unsigned char *data, int width, int height);
		void   loadFile(const std::string &filename);
		GLuint bind(GLuint slot) const;

	protected:
		void flip(unsigned *pixels, unsigned width, unsigned height);
		void destroy();
		void release();

	private:
		GLuint mHandle;
	};

}}}

#endif