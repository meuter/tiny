#ifndef __TINY_RENDERING_GL_BUFFER_H__
#define __TINY_RENDERING_GL_BUFFER_H__

#include <GL/glew.h>
#include <vector>

#include "handle.h"

namespace tiny { namespace rendering { namespace gl {

	class BufferObject 
	{
	public:	
		BufferObject() : mSize(0)
		{
			glGenBuffers(1, &mHandle);
			if (mHandle == 0)
				throw std::runtime_error("could not create vertex buffer object");
		}

		BufferObject(BufferObject &&other) = default;

		virtual ~BufferObject() = default;
		
		BufferObject &operator=(BufferObject &&other) = default;

		void loadIndices(const std::vector<unsigned int> &indices)
		{
			load(GL_ELEMENT_ARRAY_BUFFER, indices);
		}

		void loadAttribute(GLuint location, const std::vector<float> &vertexData, size_t nPerVertex)
		{
			load(GL_ARRAY_BUFFER, vertexData);
			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, nPerVertex, GL_FLOAT, GL_FALSE, nPerVertex * sizeof(vertexData[0]), (GLvoid *)0);
		}

		inline size_t size()   const { return mSize;   }
		inline GLuint handle() const { return mHandle; }

	protected:	

		template<typename T>
		void load(GLenum target, const std::vector<T> data, GLenum usage = GL_STATIC_DRAW)
		{
			mSize = data.size();
			glBindBuffer(target, mHandle);
			glBufferData(target, data.size()*sizeof(data[0]), &data[0], usage);
		}
	
		static void destroy(GLuint handle)
		{
			glDeleteBuffers(1, &handle);
		}

	private:	
		Handle<destroy> mHandle;
		size_t mSize;
	};

}}}

#endif