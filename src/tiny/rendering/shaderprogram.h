#ifndef __TINY_RENDERING_SHADER_PROGRAM_H__
#define __TINY_RENDERING_SHADER_PROGRAM_H__

#include <GL/glew.h>
#include <string>
#include <vector>
#include <initializer_list>
#include "shader.h"

namespace tiny { namespace rendering {

	class ShaderProgram
	{
	public:
		static ShaderProgram fromFiles(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename);

		ShaderProgram();
		ShaderProgram(const ShaderProgram &other) = delete;
		ShaderProgram(ShaderProgram &&other);
		virtual ~ShaderProgram();

		ShaderProgram &operator=(const ShaderProgram &other) = delete;
		ShaderProgram &operator=(ShaderProgram &&other);

		void add(Shader &&shader);
		void link();
		void use();		
	private:

		void checkProgramError(GLenum linkingStage);

		GLuint mProgramHandle;
		std::vector<Shader> mShaders;
	};

}}

#endif