#include "shaderprogram.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <utility>


namespace tiny { namespace rendering {

ShaderProgram ShaderProgram::fromFiles(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename)
{
	auto result = ShaderProgram();
	result.add(Shader::fromFile(GL_VERTEX_SHADER,   vertexShaderFilename));
	result.add(Shader::fromFile(GL_FRAGMENT_SHADER, fragmentShaderFilename));
	result.link();
	return result;
}

ShaderProgram::ShaderProgram() : mProgramHandle(glCreateProgram())
{
	if (mProgramHandle == 0)
		throw std::runtime_error("could not create program");
}

ShaderProgram::ShaderProgram(ShaderProgram &&other) : mProgramHandle(other.mProgramHandle)
{
	other.mProgramHandle = 0;
}

ShaderProgram::~ShaderProgram()
{
	destroy();
}

ShaderProgram &ShaderProgram::operator=(ShaderProgram &&other)
{
	destroy();
	mProgramHandle = other.mProgramHandle;
	other.mProgramHandle = 0;
	return (*this);
}

void ShaderProgram::add(Shader &&shader)
{	
	glAttachShader(mProgramHandle, shader.getHandle());
	mShaders.push_back(std::move(shader));
}

void ShaderProgram::use()
{
	glUseProgram(mProgramHandle);
}

void ShaderProgram::link()
{
	glLinkProgram(mProgramHandle);
	checkProgramError(GL_LINK_STATUS);

	glValidateProgram(mProgramHandle);
	checkProgramError(GL_VALIDATE_STATUS);
}

void ShaderProgram::checkProgramError(GLenum linkingStage)
{
	GLint success;

	glGetProgramiv(mProgramHandle, linkingStage, &success);

	if (!success)
	{
		char errorMessage[1024];
		glGetProgramInfoLog(mProgramHandle, sizeof(errorMessage), NULL, errorMessage);

		throw std::runtime_error("shader program error:\n" + std::string(errorMessage));
	}

}

void ShaderProgram::destroy()
{
	if (mProgramHandle != 0)
	{
		for (Shader &shader : mShaders)
			glDetachShader(mProgramHandle, shader.getHandle());
		glDeleteProgram(mProgramHandle);
	}
}

}}
