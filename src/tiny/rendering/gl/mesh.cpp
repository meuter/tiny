#include <stdexcept>
#include <fstream>
#include <iostream>

#include <tiny/utils/strutils.h>
#include <tiny/math/matrix.h>
#include <tiny/contrib/tiny_obj_loader.h>

#include "mesh.h"
#include "shaderprogram.h"

namespace tiny { namespace rendering { namespace gl {

	Mesh::Mesh() 
	{
		glGenVertexArraysAPPLE(1, &mHandle);
		if (mHandle == 0)
			throw std::runtime_error("could not create vertex array");

		glBindVertexArrayAPPLE(mHandle);
	}

	void Mesh::draw() const
	{
		glBindVertexArrayAPPLE(mHandle);
		glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
	}


	std::vector<float> computeNormals(tinyobj::mesh_t &mesh)
	{		
		using vec2 = core::vec2;
		using vec3 = core::vec3;

		std::vector<float> normals;
		std::vector<vec3> normv;

		normv.resize(mesh.positions.size()/3, vec3(0,0,0));

		for(unsigned i = 0; i < mesh.indices.size() / 3; i++) 
		{
			int i1 = mesh.indices[3*i + 0];
			int i2 = mesh.indices[3*i + 1];
			int i3 = mesh.indices[3*i + 2];

			vec3 v1(mesh.positions[i1 * 3], mesh.positions[i1 * 3 + 1], mesh.positions[i1 * 3 + 2]);
			vec3 v2(mesh.positions[i2 * 3],	mesh.positions[i2 * 3 + 1],	mesh.positions[i2 * 3 + 2]);
			vec3 v3(mesh.positions[i3 * 3],	mesh.positions[i3 * 3 + 1],	mesh.positions[i3 * 3 + 2]);

	        vec3 normal = normalize(cross(v2-v1, v3-v1));
  
	        normv[i1] += normal;
        	normv[i2] += normal;
        	normv[i3] += normal;
 		}

		for(auto n: normv)
		{
            n = normalize(n);

			normals.push_back(n.x);
            normals.push_back(n.y);
            normals.push_back(n.z);
		}

		return normals;
	}


	std::vector<float> computeTangent(const tinyobj::mesh_t &mesh)
	{		
		using vec2 = core::vec2;
		using vec3 = core::vec3;

		std::vector<float> tangents;
		std::vector<vec3> tanv;

		tanv.resize(mesh.positions.size()/3, vec3(0,0,0));

		for(unsigned i = 0; i < mesh.indices.size() / 3; i++) 
		{
			int i1 = mesh.indices[3*i + 0];
			int i2 = mesh.indices[3*i + 1];
			int i3 = mesh.indices[3*i + 2];

			vec3 v1(mesh.positions[i1 * 3], mesh.positions[i1 * 3 + 1], mesh.positions[i1 * 3 + 2]);
			vec3 v2(mesh.positions[i2 * 3],	mesh.positions[i2 * 3 + 1],	mesh.positions[i2 * 3 + 2]);
			vec3 v3(mesh.positions[i3 * 3],	mesh.positions[i3 * 3 + 1],	mesh.positions[i3 * 3 + 2]);

			vec2 uv1(mesh.texcoords[i1 * 2], mesh.texcoords[i1 * 2 + 1]);
			vec2 uv2(mesh.texcoords[i2 * 2], mesh.texcoords[i2 * 2 + 1]);
			vec2 uv3(mesh.texcoords[i3 * 2], mesh.texcoords[i3 * 2 + 1]);

	        vec3 edge1 = v2 - v1;
	        vec3 edge2 = v3 - v1;
 
		 	vec2 duv1 = uv2 - uv1;
        	vec2 duv2 = uv3 - uv1;
 
 			float dividend = (duv1.x * duv2.y - duv2.x * duv1.y);
        	float factor = (dividend == 0.0f) ? 0.0f : 1.0f/dividend;
 
        	vec3 tangent = vec3(factor * (duv2.y * edge1.x - duv1.y * edge2.x),
        						factor * (duv2.y * edge1.y - duv1.y * edge2.y), 
        						factor * (duv2.y * edge1.z - duv1.y * edge2.z));

	        tanv[i1] += tangent;
        	tanv[i2] += tangent;
        	tanv[i3] += tangent;
		}

		for(auto t: tanv) 
		{
			t = normalize(t);

            tangents.push_back(t.x);
            tangents.push_back(t.y);
            tangents.push_back(t.z);
		}

		return tangents;
	}




	Mesh &Mesh::fromFile(const std::string &objFilename, int shape)
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

		if (meshData.normals.size() != meshData.positions.size())
			meshData.normals = computeNormals(meshData);

		meshData.texcoords.resize(nVertices * 2, 0);

		mAttributes[POSITION].loadAttribute(POSITION, meshData.positions, 3);
		mAttributes[TEXCOORD].loadAttribute(TEXCOORD, meshData.texcoords, 2);
		mAttributes[NORMAL].loadAttribute(NORMAL, meshData.normals, 3);
		mAttributes[TANGENT].loadAttribute(TANGENT, computeTangent(meshData), 3);
 		mIndices.loadIndices(meshData.indices);

		return (*this);
	}

	Mesh &Mesh::fromData(const std::vector<float> positions, std::vector<float> texcoord, std::vector<unsigned int> indices)
	{
		tinyobj::mesh_t meshData;

		meshData.positions = positions;
		meshData.texcoords = texcoord;
		meshData.indices = indices;

		mAttributes[POSITION].loadAttribute(POSITION, meshData.positions, 3);
		mAttributes[TEXCOORD].loadAttribute(TEXCOORD, meshData.texcoords, 2);
		mAttributes[NORMAL].loadAttribute(NORMAL, computeNormals(meshData), 3);
		mAttributes[TANGENT].loadAttribute(TANGENT, computeTangent(meshData), 3);
 		mIndices.loadIndices(meshData.indices);

		return (*this);
	}

	Mesh &Mesh::fromFiles(const std::string &objFilname, const std::string &mtlFilename)
	{
		fromFile(objFilname);
		material().fromFile(mtlFilename);
		return (*this);
	}

	void Mesh::destroy(GLuint handle)
	{
		glDeleteVertexArrays(1, &handle);
	}

}}}
	