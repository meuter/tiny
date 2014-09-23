#include "shader.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <regex>
#include <tiny/utils/strutils.h>

#include <iostream>

namespace tiny { namespace rendering { namespace gl {

std::string loadSource(const std::string &filename)
{
	static const std::regex includeMatcher("#include[\t ]+\"([a-zA-z\\.]+)\"[\t ]*");
	std::ifstream fileStream(filename.c_str());
	std::stringstream result;
	std::smatch match;
	std::string line;
	std::string path = tiny::utils::fileDirectory(filename);

	if (fileStream.fail())
		throw std::runtime_error("could not open " + filename);

	while (getline(fileStream, line))
	{
		if (regex_match(line, match, includeMatcher))
			result << loadSource(path + match[1].str());
		else			
			result << line << std::endl;
	}

	return result.str();
}

Shader Shader::fromFile(GLenum shaderType, const std::string filename)
{
	return Shader(shaderType, loadSource(filename));
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
