#include <stdexcept>
#include <fstream>
#include <iostream>

#include <tiny/utils/strutils.h>

#include "mesh.h"
#include "shaderprogram.h"

namespace tiny { namespace rendering { namespace gl {

Mesh Mesh::fromFile(const std::string &objFilename, int shape)
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string error = tinyobj::LoadObj(shapes, materials, objFilename.c_str());

	if (!error.empty())
		throw std::runtime_error("could not load '"+objFilename+"':" + error);

	if (shape >= shapes.size())
		throw std::runtime_error("could not find shape "+std::to_string(shape)+" in '"+objFilename+"'");

	if (materials.size() != 1)
		throw std::runtime_error("more than one material found in '"+objFilename+"'");

	auto &meshData = shapes[shape].mesh;
	unsigned int nVertices = meshData.positions.size()/3;

	while (meshData.normals.size()/3 < nVertices)
		meshData.normals.emplace_back(0);	

	while (meshData.texcoords.size()/2 < nVertices)
		meshData.texcoords.emplace_back(0);	

	return Mesh(meshData);
}

Mesh Mesh::fromFiles(const std::string &objFilname, const std::string &mtlFilename)
{
	Mesh result = Mesh::fromFile(objFilname);
	result.mMaterial = Material::fromFile(mtlFilename);
	return result;
}

Mesh::Mesh(const tinyobj::mesh_t &meshData) 
{
	glGenVertexArraysAPPLE(1, &mHandle);
	if (mHandle == 0)
		throw std::runtime_error("could not create vertex array");

	glBindVertexArrayAPPLE(mHandle);
	mAttributes[POSITION].loadAttribute(POSITION, meshData.positions, 3);
	mAttributes[TEXCOORD].loadAttribute(TEXCOORD, meshData.texcoords, 2);
	mAttributes[NORMAL].loadAttribute(NORMAL, meshData.normals, 3);
	mIndices.loadIndices(meshData.indices);
}

void Mesh::draw() const
{
	glBindVertexArrayAPPLE(mHandle);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::destroy(GLuint handle)
{
	glDeleteVertexArrays(1, &handle);
}

}}}
	