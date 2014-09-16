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

VertexArray::~VertexArray()
{
	destroy();
}

VertexArray &VertexArray::operator=(VertexArray &&other) 
{
	destroy();
	mHandle = other.mHandle;
	other.release();
	return (*this);
}

void VertexArray::release()
{
	mHandle = 0;
}

void VertexArray::destroy()
{
	if (mHandle)
	{
		glDeleteVertexArrays(1, &mHandle);
		mHandle = 0;
	}
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