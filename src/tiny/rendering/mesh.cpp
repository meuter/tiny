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
	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<core::vec3> positions, normals;
	std::vector<core::vec2> texcoords;
	std::ifstream file;
	std::string line;

	auto addVertex = [&](const std::string &faceToken) -> int
	{
		auto splitted = utils::split(faceToken, '/');
		vertex newVertex;

		if (splitted.size() == 1)
			newVertex = vertex{positions[std::stoi(splitted[0])-1], core::vec2(0,0), core::vec3(0,0,0) };
		else if (splitted.size() == 2)
			newVertex = vertex{positions[std::stoi(splitted[0])-1], texcoords[std::stoi(splitted[1])-1], core::vec3(0,0,0) };
		else if (splitted.size() == 3)
			newVertex = vertex{positions[std::stoi(splitted[0])-1], texcoords[std::stoi(splitted[1])-1], normals[std::stoi(splitted[2])-1] };
		else
			throw std::runtime_error("do not know how to parse '"+faceToken+"'");

		indices.push_back(vertices.size());
		vertices.push_back(newVertex);

		return vertices.size()-1;
	};

	auto computeNormal = [](vertex &v1, vertex &v2, vertex &v3) -> core::vec3
	{
		auto d1 = normalize(v2.position-v1.position);
		auto d2 = normalize(v3.position-v1.position);

		return normalize(cross(d1,d2));
	};

	auto addFace = [&](const std::string &token1, const std::string &token2, const std::string &token3)
	{
		auto i1 = addVertex(token1);
		auto i2 = addVertex(token2);
		auto i3 = addVertex(token3);

		if (normals.size() == 0)
		{
			auto normal = computeNormal(vertices[i1], vertices[i2], vertices[i3]);
			vertices[i1].normal = normal;
			vertices[i2].normal = normal;
			vertices[i3].normal = normal;
		}
	};

	if (utils::toupper(utils::split(filename, '.').back()) != "OBJ")
		throw std::runtime_error("onlye obj files are supperted");

	file.open(filename);

	if (!file)
		throw std::runtime_error("could not open '" + filename + "'");

	while (getline(file, line))
	{
		auto tokens = utils::split(line);

		if (tokens[0] == "v")
		{
			positions.emplace_back(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
		}
		if (tokens[0] == "vt")
		{
			texcoords.emplace_back(std::stof(tokens[1]), std::stof(tokens[2]));	
		}
		if (tokens[0] == "vn")
		{	
			normals.emplace_back(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]));
		}
		else if (tokens[0] == "f")
		{
			if (tokens.size() != 4)
				std::runtime_error("only triangles are supported");
			
			addFace(tokens[1], tokens[2], tokens[3]);
		}
	}

	result.load(vertices, indices);

	return result;
}

Mesh::Mesh() : mLoaded(false)
{
}

Mesh::Mesh(const std::vector<vertex> &vertices, const std::vector<unsigned int> &indices)
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

void Mesh::load(const std::vector<vertex> &vertices, const std::vector<unsigned int> &indices) 
{
	mSize = indices.size();

	for (int i = 0; i < indices.size(); ++i)
	{
		std::cout << i << " " << indices[i] << std::endl;
	}

	glGenVertexArrays(1, &mVertexArrayHandle);
	glBindVertexArray(mVertexArrayHandle);

	glGenBuffers(1, &mVertexBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(AttributeLocation::POSITION);
	glVertexAttribPointer(AttributeLocation::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)0);
	glEnableVertexAttribArray(AttributeLocation::TEXCOORD);
	glVertexAttribPointer(AttributeLocation::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)sizeof(core::vec3));				
	glEnableVertexAttribArray(AttributeLocation::NORMAL);
	glVertexAttribPointer(AttributeLocation::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)(sizeof(core::vec3)+sizeof(core::vec2)) );				


	glGenBuffers(1, &mIndexBufferHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);
	
	mLoaded = true;
}

void Mesh::unload()
{
	if (mLoaded)
	{
		glDeleteBuffers(1, &mVertexBufferHandle);
		glDeleteVertexArrays(1, &mVertexArrayHandle);
		glDeleteVertexArrays(1, &mIndexBufferHandle);
	}
}

void Mesh::draw()
{
	glBindVertexArray(mVertexArrayHandle);
	glDrawElements(GL_TRIANGLES, mSize, GL_UNSIGNED_INT, 0);
}

}}
	