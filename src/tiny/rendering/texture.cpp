#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include "stb_image.h"


namespace tiny { namespace rendering {

Texture Texture::fromFile(const std::string &filename)
{
	Texture result;
	result.loadFile(filename);
	return result;
}

Texture::Texture()
{
	glGenTextures(1, &mTextureHandle);

	if (mTextureHandle == 0)
		throw std::runtime_error("could not generate new texture");

	bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	    
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::Texture(Texture &&other) 
{
	mTextureHandle = other.mTextureHandle;
	other.mTextureHandle = 0;
}

Texture::~Texture() 
{
	if (mTextureHandle != 0)
		glDeleteTextures(1, &mTextureHandle);
}

Texture &Texture::operator=(Texture &&other)
{
	mTextureHandle = other.mTextureHandle;
	other.mTextureHandle = 0;
	return (*this);
}

void Texture::loadData(unsigned char *data, int width, int height) 
{
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void Texture::loadFile(const std::string &filename) 
{
	int width, height, nComponents;
	unsigned char *pixels = stbi_load(filename.c_str(), &width, &height, &nComponents, 4);

	if (pixels == NULL)
		throw std::runtime_error("could not load texture file '"+filename+"'");

	loadData(pixels, width, height);
	stbi_image_free(pixels);
}

void Texture::bind() 
{
	glBindTexture(GL_TEXTURE_2D, mTextureHandle);
}



}}