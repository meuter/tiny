#include "shader.h"
#include <stdexcept>
#include <fstream>
#include <sstream>

namespace tiny { namespace rendering { namespace gl {

Shader Shader::fromFile(GLenum shaderType, const std::string filename)
{
	auto shader = Shader(shaderType);
	shader.loadFile(filename);
	shader.compile();
	return shader;
}

Shader::Shader(GLenum shaderType) : mHandle(glCreateShader(shaderType))
{
	if (mHandle == 0)
		throw std::runtime_error("could not create shader");
}

Shader::Shader(Shader &&other) : mHandle(other.handle())
{
	other.release();
}

Shader::~Shader()
{
	destroy();
}

Shader &Shader::operator=(Shader &&other)
{
	destroy();
	mHandle = other.handle();
	other.release();

	return (*this);
}

void Shader::load(const std::string &shaderSource)
{
	const GLchar *sources[] = { shaderSource.c_str() };
	const GLint   lengths[] = { static_cast<GLint>(shaderSource.length()) };

	glShaderSource(mHandle, 1, sources, lengths);
}


void Shader::loadFile(const std::string &filename)
{
	std::ifstream fileStream(filename.c_str());

	if (fileStream.fail())
		throw std::runtime_error("could not open " + filename);

	std::stringstream buffer;
	buffer << fileStream.rdbuf();

	load(buffer.str());
}

void Shader::compile() 
{
	GLint success;

	glCompileShader(mHandle);
	glGetShaderiv(mHandle, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char errorMessage[1024];
		glGetShaderInfoLog(mHandle, sizeof(errorMessage), NULL, errorMessage);
		throw std::runtime_error("shader error:\n" + std::string(errorMessage));
	}
}

void Shader::release()
{
	mHandle = 0;
}

void Shader::destroy()
{
	if (mHandle)
		glDeleteShader(mHandle);
}


}}}
