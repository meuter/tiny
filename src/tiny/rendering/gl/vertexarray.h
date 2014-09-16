#ifndef __TINY_RENDERING_GL_VERTEX_ARRAY_H__
#define __TINY_RENDERING_GL_VERTEX_ARRAY_H__

#include <GL/glew.h>
#include <stdexcept>

namespace tiny { namespace rendering { namespace gl {

	class VertexArray
	{
	public:	
		VertexArray();
		VertexArray(const VertexArray &other) = delete;
		VertexArray(VertexArray &&other);

		virtual ~VertexArray();

		VertexArray &operator=(const VertexArray &other) = delete;
		VertexArray &operator=(VertexArray &&other);

		void bind() const;
		void unbind() const;

	protected:
		void release();
		void free();

	private:
		GLuint mHandle;
	};


}}}

#endif