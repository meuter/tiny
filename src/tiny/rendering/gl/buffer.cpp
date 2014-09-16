#include "buffer.h"

namespace tiny { namespace rendering { namespace gl {

Buffer Buffer::CreateVBO(GLuint location, const std::vector<float> &vertexData, size_t nPerVertex)
{
	Buffer result;

	glBindBuffer(GL_ARRAY_BUFFER, result.mHandle);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(vertexData[0]), &vertexData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, nPerVertex, GL_FLOAT, GL_FALSE, nPerVertex * sizeof(vertexData[0]), (GLvoid *)0);

	return result;
}

Buffer Buffer::CreateIBO(const std::vector<unsigned int> &indices)
{
	Buffer result;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.mHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

	return result;
}


Buffer::Buffer() : mHandle(0)
{
	glGenBuffers(1, &mHandle);
	if (mHandle == 0)
		throw std::runtime_error("could not create vertex buffer object");
}

Buffer::Buffer(Buffer &&other) : mHandle(other.mHandle)
{
	other.release();
}

Buffer::~Buffer()
{
	if (mHandle)
		free();
}

Buffer &Buffer::operator=(Buffer &&other)
{
	free();
	mHandle = other.mHandle;
	other.release();
	return (*this);
}

void Buffer::release()
{
	mHandle = 0;
}

void Buffer::free()
{
	glDeleteBuffers(1, &mHandle);
	mHandle = 0;
}

}}}