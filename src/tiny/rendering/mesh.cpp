#include "mesh.h"
#include "shaderprogram.h"
#include "tiny_obj_loader.h"
#include <stdexcept>
#include "../utils/split.h"
#include "../utils/upper.h"

#include <iostream>

namespace tiny { namespace rendering {

Mesh Mesh::fromFile(const std::string &filename)
{
	Mesh result;

	if (utils::toupper(utils::split(filename, '.').back()) != "OBJ")
		throw std::runtime_error("onlye obj files are supperted");

	std::vector<tinyobj::shape_t> shapes;
	std::string error = tinyobj::LoadObj(shapes, filename.c_str());
	if (!error.empty())
		throw std::runtime_error(error);

	for (auto &shape: shapes)
	{
		if (shape.mesh.indices.size() % 3 != 0)
			throw std::runtime_error("invalid indices");

		if (shape.mesh.positions.size() % 3 != 0)
			throw std::runtime_error("invalid positions");

		if (shape.mesh.texcoords.size() % 2 != 0)
			throw std::runtime_error("invalid texture coordinates");

		for (size_t i = 0; i < shape.mesh.positions.size()/3; i++)
		{
			core::vec3 pos(shape.mesh.positions[i], shape.mesh.positions[i+1], shape.mesh.positions[i+2]);

			std::cout << pos << std::endl;
		}

	}

	return result;
}

Mesh::Mesh() : mLoaded(false)
{
}

Mesh::Mesh(const std::vector<vertex> &vertices, const std::vector<int> &indices)
{
	load(vertices, indices);
}


Mesh::Mesh(Mesh &&other)
	: mSize(other.mSize), mVertexArrayHandle(other.mVertexArrayHandle),
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
	mSize = other.mSize;
	mVertexArrayHandle = other.mVertexArrayHandle;
	mVertexBufferHandle = other.mVertexBufferHandle;
	mLoaded = other.mLoaded;

	other.mLoaded = false;

	return (*this);
}

void Mesh::load(const std::vector<vertex> &vertices, const std::vector<int> &indices) 
{
	mSize = indices.size();

	glGenVertexArrays(1, &mVertexArrayHandle);
	glBindVertexArray(mVertexArrayHandle);

	glGenBuffers(1, &mVertexBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(AttributeLocation::POSITION);
	glVertexAttribPointer(AttributeLocation::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)0);
	glEnableVertexAttribArray(AttributeLocation::TEXTURE_COORD);
	glVertexAttribPointer(AttributeLocation::TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)sizeof(core::vec3));				

	glGenBuffers(1, &mIndexBufferHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);
	
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

void Mesh::draw()
{
	glBindVertexArray(mVertexArrayHandle);
	glDrawElements(GL_TRIANGLES, mSize, GL_UNSIGNED_INT, 0);
	// glDrawArrays(GL_TRIANGLES, 0, mNumberOfVertices);
}

}}
	