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

	std::vector<float> computeTangent(const tinyobj::mesh_t &meshData)
	{
		using vec2 = core::vec2;
		using vec3 = core::vec3;
		using vec4 = core::vec4;

		std::vector<float> tangents;
		std::vector<vec3> tan1;
		size_t nVertices = meshData.positions.size() / 3;
		size_t nTriangles = meshData.indices.size() / 3;

		tan1.resize(nVertices, vec3(0,0,0));

		for (size_t i = 0; i < nTriangles; i+= 3)
		{
			int i1 = meshData.indices[i];
            int i2 = meshData.indices[i+1];
            int i3 = meshData.indices[i+2];

            vec3 v1(meshData.positions[3*i1+0], meshData.positions[3*i1+1], meshData.positions[3*i1+2]);
            vec3 v2(meshData.positions[3*i2+0], meshData.positions[3*i2+1], meshData.positions[3*i2+2]);
            vec3 v3(meshData.positions[3*i3+0], meshData.positions[3*i3+1], meshData.positions[3*i3+2]);

            vec2 w1(meshData.texcoords[2*i1+0], meshData.texcoords[2*i1+1]);
            vec2 w2(meshData.texcoords[2*i2+0], meshData.texcoords[2*i2+1]);
            vec2 w3(meshData.texcoords[2*i3+0], meshData.texcoords[2*i3+1]);

            float x1 = v2.x - v1.x;
            float x2 = v3.x - v1.x;
            float y1 = v2.y - v1.y;
            float y2 = v3.y - v1.y;
            float z1 = v2.z - v1.z;
            float z2 = v3.z - v1.z;

            float s1 = w2.x - w1.x;
            float s2 = w3.x - w1.x;
            float t1 = w2.y - w1.y;
            float t2 = w3.y - w1.y;

            float d = (s1 * t2 - s2 * t1);
            float r = (d == 0.0f) ? 0.0f : 1.0f/ d;
 
            vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
            vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);
 
            tan1[i1] += sdir;
            tan1[i2] += sdir;
            tan1[i3] += sdir;
		}

		for (size_t i = 0; i < meshData.positions.size(); i+= 3)
        {
			vec3 n = vec3(meshData.normals[3*i+0], meshData.normals[3*i+1], meshData.normals[3*i+2]);
            vec3 t = tan1[i/3];
            vec3 tmp = normalize(t - n * dot(n,t));

            tangents.push_back(tmp.x);
            tangents.push_back(tmp.y);
            tangents.push_back(tmp.z);
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

		// FIXME compute normals if missing
		meshData.normals.resize(nVertices * 3, 0);
		meshData.texcoords.resize(nVertices * 2, 0);

		mAttributes[POSITION].loadAttribute(POSITION, meshData.positions, 3);
		mAttributes[TEXCOORD].loadAttribute(TEXCOORD, meshData.texcoords, 2);
		mAttributes[NORMAL].loadAttribute(NORMAL, meshData.normals, 3);
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
	