#ifndef __TINY_RENDERING_GL_SHADER_H__
#define __TINY_RENDERING_GL_SHADER_H__

#include <GL/glew.h>
#include <string>
#include <map>
#include "handle.h"

namespace tiny { namespace rendering { namespace gl {

	class Shader
	{
	public:
		using Constants = std::map<std::string, std::string>;
		
		static Shader fromFile(GLenum shaderType, const std::string filename, Constants constants = Constants());

		Shader(GLenum shaderType, const std::string &shaderText);
		Shader(Shader &&other) = default;

		virtual ~Shader() = default;

		Shader &operator=(Shader &&other) = default;

		inline GLuint handle() const { return mHandle; }

	protected:	
		static void destroy(GLuint handle);

	private:
		Handle<destroy> mHandle;
	};

}}}

#endif