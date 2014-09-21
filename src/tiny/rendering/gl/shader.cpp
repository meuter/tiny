#include "shader.h"
#include <stdexcept>
#include <fstream>
#include <sstream>

namespace tiny { namespace rendering { namespace gl {

Shader Shader::fromFile(GLenum shaderType, const std::string filename)
{
	std::ifstream fileStream(filename.c_str());

	if (fileStream.fail())
		throw std::runtime_error("could not open " + filename);

	std::stringstream buffer;
	buffer << fileStream.rdbuf();

	return Shader(shaderType, buffer.str());
}

Shader::Shader(GLenum shaderType, const std::string &shaderText) : mHandle(glCreateShader(shaderType))
{
	GLint success;
	const GLchar *sources[] = { shaderText.c_str() };
	const GLint   lengths[] = { static_cast<GLint>(shaderText.length()) };

	if (mHandle == 0)
		throw std::runtime_error("could not create shader");

	glShaderSource(mHandle, 1, sources, lengths);
	glCompileShader(mHandle);
	glGetShaderiv(mHandle, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char errorMessage[1024];
		glGetShaderInfoLog(mHandle, sizeof(errorMessage), NULL, errorMessage);
		throw std::runtime_error("shader error:\n" + std::string(errorMessage));
	}
}

void Shader::destroy(GLuint handle)
{
	glDeleteShader(handle);
}


}}}
