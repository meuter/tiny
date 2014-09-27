#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include <tiny/contrib/stb_image.h>
#undef STBI_ASSERT
#undef STB_IMAGE_IMPLEMENTATION


namespace tiny { namespace rendering { namespace gl {

	Texture::Texture()
	{
		create(mHandle);
	}

	Texture::Texture(const std::string &filename, GLenum filter)
	{
		create(mHandle);
		fromFile(filename, filter);
	}

	Texture::Texture(byte *pixels, size_t width, size_t height, GLenum filter) 
	{
		create(mHandle);
		fromPixels(pixels, width, height, filter);
	}

	Texture::Texture(const vec3 &color)
	{
		create(mHandle);
		fromColor(color);
	}

	Texture &Texture::fromPixels(byte *pixels, size_t width, size_t height, GLenum filter)
	{
		create(mHandle);

		glBindTexture(GL_TEXTURE_2D, mHandle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		    
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		if (filter == GL_LINEAR_MIPMAP_LINEAR  || 
			filter == GL_LINEAR_MIPMAP_NEAREST || 
			filter == GL_NEAREST_MIPMAP_LINEAR || 
			filter == GL_NEAREST_MIPMAP_NEAREST)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
		}

		return (*this);
	}

	Texture &Texture::fromFile(const std::string &filename, GLenum filter)
	{
		int width, height, nComponents;
		byte *pixels = stbi_load(filename.c_str(), &width, &height, &nComponents, 4);

		if (pixels == NULL)
			throw std::runtime_error("could not load texture file '"+filename+"'");

		fromPixels(pixels, width, height, filter);

		stbi_image_free(pixels);

		return (*this);
	}

	Texture &Texture::fromColor(const vec3 &color)
	{
		byte r = (byte)(color.x * 255.0);
		byte g = (byte)(color.y * 255.0);
		byte b = (byte)(color.z * 255.0);
		byte a = 0xFF;
		std::array<uint32_t,16*16> pixels;
		pixels.fill(a << 24 | b << 16 | g << 8 | r);
		
		return fromPixels(reinterpret_cast<byte *>(&pixels[0]), 16, 16, GL_NEAREST);
	}

	void Texture::create(GLuint &handle)
	{
		glGenTextures(1, &handle);

		if (handle == 0) 
			throw std::runtime_error("could not generate new texture");
	}

	void Texture::destroy(GLuint handle)
	{
		glDeleteTextures(1, &handle);
	}

	void Texture::bind(GLuint slot) const
	{	
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, mHandle);
	}


}}}