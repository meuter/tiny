#ifndef __TINY_RENDERING_GL_SHADER_H__
#define __TINY_RENDERING_GL_SHADER_H__

#include <GL/glew.h>
#include <string>
#include <boost/noncopyable.hpp>

namespace tiny { namespace rendering { namespace gl {

	class Shader : boost::noncopyable
	{
	public:
		static Shader fromFile(GLenum shaderType, const std::string filename);

		Shader(GLenum shaderType);
		Shader(Shader &&other);

		virtual ~Shader();

		Shader &operator=(Shader &&other);

		void load(const std::string &shaderSource);
		void loadFile(const std::string &filename);
		void compile();

		inline GLuint handle() const { return mHandle; }

	protected:	
		void release();
		void destroy();

	private:
		GLuint mHandle;
	};

}}}

#endif