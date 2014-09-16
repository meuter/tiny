#ifndef __TINY_RENDERING_GL_VERTEX_BUFFER_H__
#define __TINY_RENDERING_GL_VERTEX_BUFFER_H__

#include <GL/glew.h>
#include <vector>

namespace tiny { namespace rendering { namespace gl {

	class VertexBuffer
	{
	public:	
		VertexBuffer();
		VertexBuffer(const VertexBuffer &other) = delete;
		VertexBuffer(VertexBuffer &&other);

		virtual ~VertexBuffer();

		VertexBuffer &operator=(const VertexBuffer &other) = delete;
		VertexBuffer &operator=(VertexBuffer &&other);

		void loadIndices(const std::vector<unsigned int> &indices);
		void loadVertexAttribute(GLuint location, const std::vector<float> &vertexData, size_t nPerVertex);

		inline size_t size() const { return mSize; }

	protected:
		void release();
		void free();

	private:	
		GLuint mHandle;
		size_t mSize;
	};


}}}

#endif