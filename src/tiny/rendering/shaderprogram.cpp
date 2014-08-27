#include "shaderprogram.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>


namespace tiny { namespace rendering {


ShaderProgram::ShaderProgram(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename)
	: mVertexShader(GL_VERTEX_SHADER, vertexShaderFilename),
	  mFragmentShader(GL_FRAGMENT_SHADER, fragmentShaderFilename)
{
	mProgramHandle = glCreateProgram();
	if (mProgramHandle == 0)
		throw std::runtime_error("could not create program");

	glAttachShader(mProgramHandle, mVertexShader.getHandle());
	glAttachShader(mProgramHandle, mFragmentShader.getHandle());

	linkProgram();
}

ShaderProgram::~ShaderProgram()
{
	glDetachShader(mProgramHandle, mVertexShader.getHandle());
	glDetachShader(mProgramHandle, mFragmentShader.getHandle());
	glDeleteProgram(mProgramHandle);
}

void ShaderProgram::use()
{
	glUseProgram(mProgramHandle);
}

void ShaderProgram::linkProgram()
{
	glLinkProgram(mProgramHandle);
	checkForErrors();
	glValidateProgram(mProgramHandle);
	checkForErrors();
}

void ShaderProgram::checkForErrors()
{
	GLint success;

	glGetProgramiv(mProgramHandle, GL_LINK_STATUS, &success);

	if (!success)
	{
		char errorMessage[1024];
		glGetProgramInfoLog(mProgramHandle, sizeof(errorMessage), NULL, errorMessage);

		throw std::runtime_error("linking error:\n" + std::string(errorMessage));
	}
}



}}
