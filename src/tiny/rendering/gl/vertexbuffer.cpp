#include "vertexbuffer.h"

namespace tiny { namespace rendering { namespace gl {

VertexBuffer::VertexBuffer() : mHandle(0), mSize(0)
{
	glGenBuffers(1, &mHandle);
	if (mHandle == 0)
		throw std::runtime_error("could not create vertex buffer object");
}

VertexBuffer::VertexBuffer(VertexBuffer &&other) : mHandle(other.mHandle), mSize(other.mSize)
{
	other.release();
}

VertexBuffer::~VertexBuffer()
{
	if (mHandle)
		free();
}

VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other)
{
	free();
	mHandle = other.mHandle;
	mSize   = other.mSize;
	other.release();
	return (*this);
}

void VertexBuffer::loadIndices(const std::vector<unsigned int> &indices)
{
	mSize = indices.size();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);
}

void VertexBuffer::loadVertexAttribute(GLuint location, const std::vector<float> &vertexData, size_t nPerVertex)
{
	mSize = vertexData.size();
	glBindBuffer(GL_ARRAY_BUFFER, mHandle);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(vertexData[0]), &vertexData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, nPerVertex, GL_FLOAT, GL_FALSE, nPerVertex * sizeof(vertexData[0]), (GLvoid *)0);
}

void VertexBuffer::release()
{
	mHandle = 0;
}

void VertexBuffer::free()
{
	glDeleteBuffers(1, &mHandle);
	mHandle = 0;
	mSize = 0;
}

}}}