#ifndef __TINY_RENDERING_GL_VERTEX_BUFFER_H__
#define __TINY_RENDERING_GL_VERTEX_BUFFER_H__

#include <GL/glew.h>
#include <vector>

#include "buffer.h"

namespace tiny { namespace rendering { namespace gl {

	class VertexBuffer : public Buffer
	{
	public:	
		VertexBuffer() = default;
		VertexBuffer(VertexBuffer &&other) = default;

		virtual ~VertexBuffer() = default;

		VertexBuffer &operator=(VertexBuffer &&other) = default;

		void load(GLuint location, const std::vector<float> &vertexData, size_t nPerVertex)
		{
			Buffer::load(GL_ARRAY_BUFFER, vertexData);
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, nPerVertex, GL_FLOAT, GL_FALSE, nPerVertex * sizeof(vertexData[0]), (GLvoid *)0);
		}
	};


}}}

#endif