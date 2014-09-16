#ifndef __TINY_RENDERING_GL_H__
#define __TINY_RENDERING_GL_H__

#include <GL/glew.h>
#include <vector>

namespace tiny { namespace rendering { namespace gl {

	class Buffer
	{
	public:	
		static Buffer CreateVBO(GLuint location, const std::vector<float> &vertexData, size_t nPerVertex);
		static Buffer CreateIBO(const std::vector<unsigned int> &indices);

		Buffer();
		Buffer(const Buffer &other) = delete;
		Buffer(Buffer &&other);

		virtual ~Buffer();

		Buffer &operator=(const Buffer &other) = delete;
		Buffer &operator=(Buffer &&other);

	protected:
		void release();
		void free();

	private:	
		GLuint mHandle;
	};


}}}

#endif