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

	glBindTexture(GL_TEXTURE_2D, mTextureHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	    
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char whilePixel[4] = { 255,255,255,255 };
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whilePixel);
}

Texture::Texture(Texture &&other) 
{
	mTextureHandle = other.mTextureHandle;
	other.mTextureHandle = 0;
}

Texture::~Texture() 
{
	destroy();
}

Texture &Texture::operator=(Texture &&other)
{
	destroy();
	mTextureHandle = other.mTextureHandle;
	other.mTextureHandle = 0;
	return (*this);
}

void Texture::loadData(unsigned char *data, int width, int height) 
{
	glBindTexture(GL_TEXTURE_2D, mTextureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void Texture::loadFile(const std::string &filename) 
{
	int width, height, nComponents;
	unsigned char *pixels = stbi_load(filename.c_str(), &width, &height, &nComponents, 4);

	flip((unsigned *)pixels, (unsigned)width, (unsigned)height);

	if (pixels == NULL)
		throw std::runtime_error("could not load texture file '"+filename+"'");

	loadData(pixels, width, height);
	stbi_image_free(pixels);
}

void Texture::bind(GLuint textureUnit) 
{
	if (textureUnit >= 32)
		throw std::runtime_error("only 32 texture units are available");

	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, mTextureHandle);
}

void Texture::flip(unsigned* buffer, unsigned width, unsigned height)
{
    unsigned rows = height / 2; // Iterate only half the buffer to get a full flip
    unsigned* tempRow = (unsigned*)malloc(width * sizeof(unsigned));

    for (unsigned rowIndex = 0; rowIndex < rows; rowIndex++)
    {
        memcpy(tempRow, buffer + rowIndex * width, width * sizeof(unsigned));
        memcpy(buffer + rowIndex * width, buffer + (height - rowIndex - 1) * width, width * sizeof(unsigned));
        memcpy(buffer + (height - rowIndex - 1) * width, tempRow, width * sizeof(unsigned));
    }

    free(tempRow);
}

void Texture::destroy()
{
	if (mTextureHandle != 0)
		glDeleteTextures(1, &mTextureHandle);
}

}}