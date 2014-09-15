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

	auto createVertex = [&](const std::string &faceToken)
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
			
			createVertex(tokens[1]);
			createVertex(tokens[2]);
			createVertex(tokens[3]);
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
	