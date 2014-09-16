#ifndef __TINY_RENDERING_GL_INDEX_BUFFER_H__
#define __TINY_RENDERING_GL_INDEX_BUFFER_H__

#include <GL/glew.h>
#include <vector>

#include "buffer.h"

namespace tiny { namespace rendering { namespace gl {

	class IndexBuffer : public Buffer
	{
	public:	
		IndexBuffer() = default;
		IndexBuffer(IndexBuffer &&other) = default;

		virtual ~IndexBuffer() = default;

		IndexBuffer &operator=(IndexBuffer &&other) = default;

		inline void load(const std::vector<unsigned int> &indices) 
		{
			Buffer::load(GL_ELEMENT_ARRAY_BUFFER, indices);
		}
	};


}}}

#endif