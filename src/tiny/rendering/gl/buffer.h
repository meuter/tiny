#ifndef __TINY_RENDERING_GL_BUFFER_H__
#define __TINY_RENDERING_GL_BUFFER_H__

#include <GL/glew.h>
#include <vector>
#include <boost/noncopyable.hpp>

namespace tiny { namespace rendering { namespace gl {

	class Buffer : boost::noncopyable
	{
	public:	
		Buffer();
		Buffer(Buffer &&other);

		virtual ~Buffer();
		
		Buffer &operator=(Buffer &&other);

		template<typename T>
		void load(GLenum target, const std::vector<T> data, GLenum usage = GL_STATIC_DRAW)
		{
			mSize = data.size();
			glBindBuffer(target, mHandle);
			glBufferData(target, data.size()*sizeof(data[0]), &data[0], usage);
		}

		inline size_t size() const   { return mSize; }
		inline GLuint handle() const { return mHandle; }

	protected:		
		void release();
		void destroy();

	private:	
		GLuint mHandle;
		size_t mSize;
	};

}}}

#endif