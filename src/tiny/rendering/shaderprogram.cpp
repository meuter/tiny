#include "shaderprogram.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>


namespace tiny { namespace rendering {

ShaderProgram::ShaderProgram(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename)
{
	mProgramHandle = glCreateProgram();
	if (mProgramHandle == 0)
		throw std::runtime_error("could not create program");

	addShader(Shader::fromFile(GL_FRAGMENT_SHADER, fragmentShaderFilename));
	addShader(Shader::fromFile(GL_VERTEX_SHADER, vertexShaderFilename));
	compile();
	link();
}

ShaderProgram::~ShaderProgram()
{
	for (Shader &shader : mShaders)
		glDetachShader(mProgramHandle, shader.getHandle());
	glDeleteProgram(mProgramHandle);
}

void ShaderProgram::addShader(Shader &&shader)
{	
	glAttachShader(mProgramHandle, shader.getHandle());
	mShaders.push_back(std::move(shader));

	std::cout << "stolen shader = " << shader.getHandle() << std::endl;

}

void ShaderProgram::compile()
{
	for (Shader &shader : mShaders)
		shader.compile();
}


void ShaderProgram::use()
{
	glUseProgram(mProgramHandle);
}

void ShaderProgram::link()
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
