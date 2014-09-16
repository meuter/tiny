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

	if (materials.size() != 1)
		throw std::runtime_error("more than one material found in '"+filename+"'");

	auto &mesh = shapes[0].mesh;
	unsigned int nVertices = mesh.positions.size()/3;

	while (mesh.normals.size()/3 < nVertices)
		mesh.normals.emplace_back(0);	

	while (mesh.texcoords.size()/2 < nVertices)
		mesh.texcoords.emplace_back(0);	

	result.load(mesh);

	return result;
}

Mesh::Mesh() : mSize(0)
{
}

Mesh::~Mesh()
{
}


void Mesh::load(const tinyobj::mesh_t &mesh) 
{
	mSize = mesh.indices.size();

	mVertexArray.bind();
	mPositions = gl::Buffer::CreateVBO(AttributeLocation::POSITION, mesh.positions, 3);
	mTexcoords = gl::Buffer::CreateVBO(AttributeLocation::TEXCOORD, mesh.texcoords, 2);
	mNormals   = gl::Buffer::CreateVBO(AttributeLocation::NORMAL, mesh.normals, 3);
	mIndices   = gl::Buffer::CreateIBO(mesh.indices);
	mVertexArray.unbind();
}

void Mesh::draw() const
{
	mVertexArray.bind();
	glDrawElements(GL_TRIANGLES, mSize, GL_UNSIGNED_INT, 0);
}

}}
	