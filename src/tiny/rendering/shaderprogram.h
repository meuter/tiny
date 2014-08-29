#ifndef __TINY_RENDERING_SHADER_PROGRAM_H__
#define __TINY_RENDERING_SHADER_PROGRAM_H__

#include <GL/glew.h>
#include <string>
#include <vector>
#include "shader.h"

namespace tiny { namespace rendering {

	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename);
		virtual ~ShaderProgram();

		void addShader(Shader &&shader);

		inline GLuint getHandle() const { return mProgramHandle; }

		void compile();
		void link();
		void use();
	private:
		void checkForErrors();

		GLuint mProgramHandle;
		std::vector<Shader> mShaders;
	};

}}

#endif