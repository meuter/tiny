#include "mesh.h"
#include "shaderprogram.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include "../utils/strutils.h"

namespace tiny { namespace rendering {

Mesh Mesh::fromFile(const std::string &filename)
{
	Mesh result;

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string error = tinyobj::LoadObj(shapes, materials, filename.c_str());

	if (!error.empty())
		throw std::runtime_error("could not load '"+filename+"':" + error);

	if (shapes.size() != 1)
		throw std::runtime_error("more than one mesh found in '"+filename+"'");

	auto &mesh = shapes[0].mesh;
	unsigned int nVertices = mesh.positions.size()/3;

	if (mesh.normals.size() / 3 != nVertices)
	{
		std::cout << "Computing Normals..." << std::endl;

		for (int f = 0; f < mesh.indices.size()/3; ++f)
		{
			int i1 = mesh.indices[3*f+0];
			int i2 = mesh.indices[3*f+1];
			int i3 = mesh.indices[3*f+2];

			core::vec3 p1(mesh.positions[3*i1+0], mesh.positions[3*i1+1], mesh.positions[3*i1+2]);
			core::vec3 p2(mesh.positions[3*i2+0], mesh.positions[3*i2+1], mesh.positions[3*i2+2]);
			core::vec3 p3(mesh.positions[3*i3+0], mesh.positions[3*i3+1], mesh.positions[3*i3+2]);

			auto d1 = normalize(p1-p2);
			auto d2 = normalize(p1-p3);
			auto normal = cross(d1,d2);

			mesh.normals.push_back(normal.x);
			mesh.normals.push_back(normal.y);
			mesh.normals.push_back(normal.z);
		}

		std::cout << "|normals| = " << mesh.normals.size() << std::endl;

	}
	std::cout << "|vertices| = " << mesh.positions.size()/3 << std::endl;

	if (mesh.texcoords.size() / 2 != nVertices)
	{
		while (mesh.texcoords.size()/2 < nVertices)
			mesh.texcoords.emplace_back(0);	
	}

	result.load(mesh);

	return result;
}

Mesh::Mesh() : mLoaded(false)
{
}

Mesh::Mesh(Mesh &&other)
	: mSize(other.mSize), mVertexArrayHandle(other.mVertexArrayHandle),
	  mPositionsBufferHandle(other.mPositionsBufferHandle), 
	  mTexcoordsBufferHandle(other.mTexcoordsBufferHandle), 
	  mNormalsBufferHandle(other.mNormalsBufferHandle), 
	  mLoaded(other.mLoaded)
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
	mPositionsBufferHandle = other.mPositionsBufferHandle;
	mTexcoordsBufferHandle = other.mTexcoordsBufferHandle;
	mNormalsBufferHandle = other.mNormalsBufferHandle;
	mLoaded = other.mLoaded;

	other.mLoaded = false;

	return (*this);
}

void Mesh::load(const tinyobj::mesh_t &mesh) 
{
	mSize = mesh.indices.size();

	glGenVertexArrays(1, &mVertexArrayHandle);
	glBindVertexArray(mVertexArrayHandle);

	auto loadVertexAttribute = [&](GLuint handle, GLuint location, const std::vector<float> bufferData)
	{
		size_t nVertices = mesh.positions.size() / 3;
		size_t nDataPerVertex = bufferData.size()/nVertices;

		glGenBuffers(1, &handle);
		glBindBuffer(GL_ARRAY_BUFFER, handle);
		glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(float), &bufferData[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, nDataPerVertex, GL_FLOAT, GL_FALSE, nDataPerVertex * sizeof(float), (GLvoid *)0);

	};

	loadVertexAttribute(mPositionsBufferHandle, AttributeLocation::POSITION, mesh.positions);
	loadVertexAttribute(mTexcoordsBufferHandle, AttributeLocation::TEXCOORD, mesh.texcoords);
	loadVertexAttribute(mNormalsBufferHandle,   AttributeLocation::NORMAL,   mesh.normals);

	glGenBuffers(1, &mIndexBufferHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(mesh.indices[0]), &mesh.indices[0], GL_STATIC_DRAW);
	
	// mLoaded = true;
}

void Mesh::unload()
{
	if (mLoaded)
	{
		glDeleteBuffers(1, &mIndexBufferHandle);
		glDeleteBuffers(1, &mNormalsBufferHandle);
		glDeleteBuffers(1, &mTexcoordsBufferHandle);
		glDeleteBuffers(1, &mPositionsBufferHandle);
		glDeleteVertexArrays(1, &mVertexArrayHandle);
	}
}

void Mesh::draw()
{
	glBindVertexArray(mVertexArrayHandle);
	glDrawElements(GL_TRIANGLES, mSize, GL_UNSIGNED_INT, 0);
}

}}
	