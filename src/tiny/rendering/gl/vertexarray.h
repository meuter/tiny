#ifndef __TINY_RENDERING_GL_VERTEX_ARRAY_H__
#define __TINY_RENDERING_GL_VERTEX_ARRAY_H__

#include <GL/glew.h>
#include <stdexcept>
#include <boost/noncopyable.hpp>

namespace tiny { namespace rendering { namespace gl {

	class VertexArray : boost::noncopyable
	{
	public:	
		VertexArray();
		VertexArray(VertexArray &&other);

		virtual ~VertexArray();

		VertexArray &operator=(VertexArray &&other);

		void bind() const;
		void unbind() const;

	protected:
		void release();
		void destroy();

	private:
		GLuint mHandle;
	};


}}}

#endif