#include "shader.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>

static void glCheckShaderError(GLint handle, GLenum status, const std::string &step)
{
	GLint success;

	glGetShaderiv(handle, status, &success);

	if (!success)
	{
		char errorMessage[1024];
		glGetShaderInfoLog(handle, sizeof(errorMessage), NULL, errorMessage);
		throw std::runtime_error(step + " error:\n" + std::string(errorMessage));
	}
}

namespace tiny { namespace rendering {


Shader::Shader(GLenum shaderType, const std::string &filename) 
{
	mShaderHandle = glCreateShader(shaderType);
	if (mShaderHandle == 0)
		throw std::runtime_error("could not create shader");

	loadShader(filename);
	compileShader();
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
	std::string shaderSource = buffer.str();

	const GLchar *sources[] = { shaderSource.c_str() };
	const GLint   lengths[] = { static_cast<GLint>(shaderSource.length()) };

	glShaderSource(mShaderHandle, 1, sources, lengths);
}

void Shader::compileShader() 
{
	glCompileShader(mShaderHandle);
	glCheckShaderError(mShaderHandle, GL_COMPILE_STATUS, "compilation");
}



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

void ShaderProgram::linkProgram()
{
	glLinkProgram(mProgramHandle);
	glCheckShaderError(mProgramHandle, GL_LINK_STATUS, "linking");
	glValidateProgram(mProgramHandle);
	glCheckShaderError(mProgramHandle, GL_LINK_STATUS, "validation");
}


}}
