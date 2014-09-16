#include "vertexarray.h"

namespace tiny { namespace rendering { namespace gl {

VertexArray::VertexArray()
{
	glGenVertexArraysAPPLE(1, &mHandle);
	if (mHandle == 0)
		throw std::runtime_error("could not create vertex array");
}

VertexArray::VertexArray(VertexArray &&other) : mHandle(other.mHandle)
{
	other.release();
}

VertexArray &VertexArray::operator=(VertexArray &&other) 
{
	free();
	mHandle = other.mHandle;
	other.release();
	return (*this);
}

VertexArray::~VertexArray()
{
	free();
}

void VertexArray::release()
{
	mHandle = 0;
}

void VertexArray::free()
{
	glDeleteVertexArrays(1, &mHandle);
	mHandle = 0;
}

void VertexArray::bind() const
{
	glBindVertexArrayAPPLE(mHandle);
}

void VertexArray::unbind() const
{
	glBindVertexArrayAPPLE(0);	
}

}}}