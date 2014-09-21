#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include <tiny/contrib/stb_image.h>
#undef STBI_ASSERT
#undef STB_IMAGE_IMPLEMENTATION


namespace tiny { namespace rendering { namespace gl {

Texture Texture::fromFile(const std::string &filename)
{
	int width, height, nComponents;
	unsigned char *pixels = stbi_load(filename.c_str(), &width, &height, &nComponents, 4);

	if (pixels == NULL)
		throw std::runtime_error("could not load texture file '"+filename+"'");

    auto result = Texture(pixels, width, height);

	stbi_image_free(pixels);

	return result;
}

Texture::Texture(unsigned char *pixels, size_t width, size_t height) 
{
	glGenTextures(1, &mHandle);

	if (mHandle == 0) 
		throw std::runtime_error("could not generate new texture");

	glBindTexture(GL_TEXTURE_2D, mHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	    
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

void Texture::destroy(GLuint handle)
{
	glDeleteTextures(1, &handle);
}

void Texture::bind(GLuint slot) const
{	
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, mHandle);
}


}}}