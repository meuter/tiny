#ifndef __TINY_RENDERING_SHADER_H__
#define __TINY_RENDERING_SHADER_H__

#include <GL/glew.h>
#include <string>

namespace tiny { namespace rendering {

	class Shader
	{
	public:
		Shader(GLenum shaderType, const std::string &filename);
		virtual ~Shader();

		inline GLuint getHandle() const { return mShaderHandle; }

	private:

		void loadShader(const std::string &filename);
		void compileShader();
		void checkForErrors();

		GLuint mShaderHandle;
	};

}}

#endif