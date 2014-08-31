#ifndef __TINY_CORE_MESH_H__
#define __TINY_CORE_MESH_H__

#include <GL/glew.h>
#include <vector>
#include <tiny/core/types.h>

namespace tiny { namespace rendering {

#define NUM_BUFFERS 1

	class Mesh
	{
		using vec3 = core::vec3;

	public:	
		Mesh()
		{
			vertices.push_back(vec3(-0.5, -0.5, 0));
			vertices.push_back(vec3( 0,   0.5,  0));
			vertices.push_back(vec3(0.5,-0.5, 0));

			glGenVertexArrays(1, &vertexArrayHandle);
			glBindVertexArray(vertexArrayHandle);

			glGenBuffers(NUM_BUFFERS, vertexArrayBufferHandles);
			glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBufferHandles[0]);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 0,0);

			glBindVertexArray(0);
		}

		~Mesh()
		{
			glDeleteVertexArrays(1, &vertexArrayHandle);
		}

		void draw()
		{
			glBindVertexArray(vertexArrayHandle);
			glDrawArrays(GL_TRIANGLES, 0, vertices.size());
			glBindVertexArray(0);

		}


		GLuint vertexArrayHandle;
		GLuint vertexArrayBufferHandles[NUM_BUFFERS];
		std::vector<vec3> vertices;
	};

}}

#endif