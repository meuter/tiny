#include "shader.h"
#include <stdexcept>
#include <fstream>
#include <sstream>


namespace tiny { namespace rendering {

Shader Shader::fromFile(GLenum shaderType, const std::string filename)
{
	auto shader = Shader(shaderType);
	shader.loadFile(filename);
	shader.compile();
	return shader;
}

Shader::Shader(GLenum shaderType) : mShaderHandle(glCreateShader(shaderType))
{
	if (mShaderHandle == 0)
		throw std::runtime_error("could not create shader");
}

Shader::Shader(Shader &&other) : mShaderHandle(other.mShaderHandle)
{
	other.mShaderHandle = 0;
}

Shader::~Shader()
{
	destroy();
}

Shader &Shader::operator=(Shader &&other)
{
	destroy();
	mShaderHandle = other.mShaderHandle;
	other.mShaderHandle = 0;

	return (*this);
}

void Shader::loadSource(const std::string &shaderSource)
{
	const GLchar *sources[] = { shaderSource.c_str() };
	const GLint   lengths[] = { static_cast<GLint>(shaderSource.length()) };

	glShaderSource(mShaderHandle, 1, sources, lengths);
}


void Shader::loadFile(const std::string &filename)
{
	std::ifstream fileStream(filename.c_str());

	if (fileStream.fail())
		throw std::runtime_error("could not open " + filename);

	std::stringstream buffer;
	buffer << fileStream.rdbuf();

	loadSource(buffer.str());
}

void Shader::compile() 
{
	GLint success;

	glCompileShader(mShaderHandle);
	glGetShaderiv(mShaderHandle, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char errorMessage[1024];
		glGetShaderInfoLog(mShaderHandle, sizeof(errorMessage), NULL, errorMessage);
		throw std::runtime_error("shader error:\n" + std::string(errorMessage));
	}
}


void Shader::destroy()
{
	if (mShaderHandle != 0)
		glDeleteShader(mShaderHandle);
}


}}
