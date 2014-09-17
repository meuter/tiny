#include "mesh.h"
#include "shaderprogram.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include "../utils/strutils.h"

namespace tiny { namespace rendering {

Mesh Mesh::fromFile(const std::string &objFilename)
{
	Mesh result;

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string error = tinyobj::LoadObj(shapes, materials, objFilename.c_str());

	if (!error.empty())
		throw std::runtime_error("could not load '"+objFilename+"':" + error);

	if (shapes.size() != 1)
		throw std::runtime_error("more than one mesh found in '"+objFilename+"'");

	if (materials.size() != 1)
		throw std::runtime_error("more than one material found in '"+objFilename+"'");

	auto &mesh = shapes[0].mesh;
	unsigned int nVertices = mesh.positions.size()/3;

	while (mesh.normals.size()/3 < nVertices)
		mesh.normals.emplace_back(0);	

	while (mesh.texcoords.size()/2 < nVertices)
		mesh.texcoords.emplace_back(0);	

	result.load(mesh);

	return result;
}

Mesh Mesh::fromFiles(const std::string &objFilname, const std::string &mtlFilename)
{
	Mesh result = Mesh::fromFile(objFilname);
	result.mMaterial = Material::fromFile(mtlFilename);
	return result;
}

void Mesh::load(const tinyobj::mesh_t &mesh) 
{
	mVertexArray.bind();
	{
		mPositions.load(AttributeLocation::POSITION, mesh.positions, 3);
		mTexcoords.load(AttributeLocation::TEXCOORD, mesh.texcoords, 2);
		mNormals.load(AttributeLocation::NORMAL, mesh.normals, 3);
		mIndices.load(mesh.indices);
	}
	mVertexArray.unbind();
}

void Mesh::draw() const
{
	mVertexArray.bind();
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
	mVertexArray.unbind();
}

}}
	