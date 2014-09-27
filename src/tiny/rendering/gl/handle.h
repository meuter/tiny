#ifndef __TINY_RENDERING_GL_HANDLE___
#define __TINY_RENDERING_GL_HANDLE___

#include <GL/glew.h>

namespace tiny { namespace rendering { namespace gl {

	template<void destroy(GLuint)>
	class Handle
	{
		using Ref      = GLuint(&);
		using ConstRef = const GLuint(&);

	public:
		Handle(GLuint value = 0) : value(value) {}
		Handle(const Handle &other) = delete;
		Handle(Handle &&other) : value(other) {	other.value = 0; }

		~Handle() { if (value) destroy(value); }

		Handle &operator=(const Handle &other) = delete;

		Handle &operator=(Handle &&other) 
		{ 
			if (value) destroy(value);
			value = other; 
			other.value = 0;
			return (*this); 
		}

		inline GLuint *operator &()	  	      { return &value; }
		inline const GLuint *operator&() const { return &value; }
		inline operator Ref()             { return value; }
		inline operator ConstRef() const  { return value; }

	private:
		GLuint value;
	};

}}}



#endif