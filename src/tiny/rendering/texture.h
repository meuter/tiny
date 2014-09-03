#ifndef __TINY_RENDERING_TEXTURE_H__
#define __TINY_RENDERING_TEXTURE_H__

#include <GL/glew.h>
#include <string>

namespace tiny { namespace rendering {

	class Texture
	{
	public:	
		static Texture fromFile(const std::string &filename);

		Texture();
		Texture(const Texture &other) = delete;
		Texture(Texture &&other);
		virtual ~Texture();

		Texture &operator=(const Texture &other) = delete;
		Texture &operator=(Texture &&other);

		inline GLuint getHandle() const { return mTextureHandle; }

		void loadData(unsigned char *data, int width, int height);
		void loadFile(const std::string &filename);
		void bind(GLuint textureUnit = 0);

	protected:
		void flip(unsigned *pixels, unsigned width, unsigned height);
		void destroy();

	private:
		GLuint mTextureHandle;
	};

}}

#endif