#include "buffer.h"

namespace tiny { namespace rendering { namespace gl {

Buffer::Buffer() : mHandle(0), mSize(0)
{
	glGenBuffers(1, &mHandle);
	if (mHandle == 0)
		throw std::runtime_error("could not create vertex buffer object");
}

Buffer::Buffer(Buffer &&other) : mHandle(other.mHandle), mSize(other.mSize)
{
	other.release();
}

Buffer::~Buffer()
{
	if (mHandle)
		destroy();
}

Buffer &Buffer::operator=(Buffer &&other)
{
	destroy();
	mHandle = other.mHandle;
	mSize   = other.mSize;
	other.release();
	return (*this);
}

void Buffer::release()
{
	mHandle = 0;
}

void Buffer::destroy()
{
	glDeleteBuffers(1, &mHandle);
	mHandle = 0;
	mSize = 0;
}

}}}