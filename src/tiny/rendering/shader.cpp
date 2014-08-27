#include "shader.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>


namespace tiny { namespace rendering {


Shader::Shader(GLenum shaderType, const std::string &filename) 
{
	mShaderHandle = glCreateShader(shaderType);
	if (mShaderHandle == 0)
		throw std::runtime_error("could not create shader");

	loadShader(filename);
	compileShader();
	checkForErrors();
}

Shader::~Shader()
{
	glDeleteShader(mShaderHandle);
}


void Shader::loadShader(const std::string &filename)
{
	std::ifstream fileStream(filename.c_str());

	if (fileStream.fail())
		throw std::runtime_error("could not open " + filename);

	std::stringstream buffer;
	buffer << fileStream.rdbuf();
	std::string shaderSource = buffer.str();

	const GLchar *sources[] = { shaderSource.c_str() };
	const GLint   lengths[] = { static_cast<GLint>(shaderSource.length()) };

	glShaderSource(mShaderHandle, 1, sources, lengths);
}

void Shader::compileShader() 
{
	glCompileShader(mShaderHandle);
}


void Shader::checkForErrors()
{
	GLint success;

	glGetShaderiv(mShaderHandle, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char errorMessage[1024];
		glGetShaderInfoLog(mShaderHandle, sizeof(errorMessage), NULL, errorMessage);
		throw std::runtime_error("shader error:\n" + std::string(errorMessage));
	}
}

}}
