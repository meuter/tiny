#ifndef __TINY_RENDERING_SHADER_PROGRAM_H__
#define __TINY_RENDERING_SHADER_PROGRAM_H__

#include <GL/glew.h>
#include <string>
#include "shader.h"

namespace tiny { namespace rendering {

	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename);
		virtual ~ShaderProgram();

		inline GLuint getHandle() const { return mProgramHandle; }

		void use();

	private:

		void linkProgram();
		void checkForErrors();


		GLuint mProgramHandle;
		Shader mVertexShader, mFragmentShader;
	};

}}

#endif