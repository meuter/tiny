#include "mesh.h"
#include "shaderprogram.h"
#include <stdexcept>

namespace tiny { namespace rendering {

Mesh::Mesh() : mLoaded(false)
{
}

Mesh::Mesh(const std::vector<vertex> &vertices)
{
	loadVertices(vertices);
}


Mesh::Mesh(Mesh &&other)
	: mNumberOfVertices(other.mNumberOfVertices), mVertexArrayHandle(other.mVertexArrayHandle),
	  mVertexBufferHandle(other.mVertexBufferHandle), mLoaded(other.mLoaded)
{
	other.mLoaded = false;
}

Mesh::~Mesh()
{
	unload();
}

Mesh &Mesh::operator=(Mesh &&other)
{
	unload();
	mNumberOfVertices = other.mNumberOfVertices;
	mVertexArrayHandle = other.mVertexArrayHandle;
	mVertexBufferHandle = other.mVertexBufferHandle;
	mLoaded = other.mLoaded;

	other.mLoaded = false;

	return (*this);
}

void Mesh::loadVertices(const std::vector<vertex> &vertices) 
{
	mNumberOfVertices = vertices.size();

	glGenVertexArrays(1, &mVertexArrayHandle);
	glBindVertexArray(mVertexArrayHandle);

	glGenBuffers(1, &mVertexBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	mLoaded = true;
}

void Mesh::unload()
{
	if (mLoaded)
	{
		glDeleteBuffers(1, &mVertexBufferHandle);
		glDeleteVertexArrays(1, &mVertexArrayHandle);
	}
}

void Mesh::draw(const ShaderProgram &shaderProgram)
{
	glBindVertexArray(mVertexArrayHandle);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 0,0);
	glDrawArrays(GL_TRIANGLES, 0, mNumberOfVertices);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

}}
	