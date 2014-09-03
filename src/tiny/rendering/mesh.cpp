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
	GLint positionAttributeLocation = shaderProgram.getAttributeLocation("position");
	GLint textureCoordAttributeLocation = shaderProgram.getAttributeLocation("textureCoord");

	glBindVertexArray(mVertexArrayHandle);
	glEnableVertexAttribArray(positionAttributeLocation);
	glEnableVertexAttribArray(textureCoordAttributeLocation);

	glVertexAttribPointer(positionAttributeLocation, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)0);
	glVertexAttribPointer(textureCoordAttributeLocation, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)sizeof(core::vec3));
	glDrawArrays(GL_TRIANGLES, 0, mNumberOfVertices);

	glDisableVertexAttribArray(positionAttributeLocation);
	glDisableVertexAttribArray(textureCoordAttributeLocation);
	glBindVertexArray(0);
}

}}
	