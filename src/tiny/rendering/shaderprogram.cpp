#include "shaderprogram.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>


namespace tiny { namespace rendering {

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
	if (mProgramHandle == 0)
		return;

	for (Shader &shader : mShaders)
		glDetachShader(mProgramHandle, shader.getHandle());
	glDeleteProgram(mProgramHandle);
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
	GLint success;

	glLinkProgram(mProgramHandle);
	glValidateProgram(mProgramHandle);
	glGetProgramiv(mProgramHandle, GL_LINK_STATUS, &success);

	if (!success)
	{
		char errorMessage[1024];
		glGetProgramInfoLog(mProgramHandle, sizeof(errorMessage), NULL, errorMessage);

		throw std::runtime_error("linking error:\n" + std::string(errorMessage));
	}
}



}}
