#include <stdexcept>
#include <fstream>
#include <sstream>
#include <utility>

#include "shaderprogram.h"
#include "mesh.h"
#include "material.h"

namespace tiny { namespace rendering { namespace gl {

ShaderProgram ShaderProgram::fromFiles(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename)
{
	auto result = ShaderProgram();
	result.addShader(Shader::fromFile(GL_VERTEX_SHADER,   vertexShaderFilename));
	result.addShader(Shader::fromFile(GL_FRAGMENT_SHADER, fragmentShaderFilename));
	result.link();
	return result;
}

ShaderProgram::ShaderProgram() : mHandle(glCreateProgram())
{
	if (mHandle == 0)
		throw std::runtime_error("could not create shader program");
}

ShaderProgram::ShaderProgram(ShaderProgram &&other) : mHandle(other.mHandle), mShaders(std::move(other.mShaders))
{
	other.release();
}

ShaderProgram::~ShaderProgram()
{
	destroy();
}

ShaderProgram &ShaderProgram::operator=(ShaderProgram &&other)
{
	destroy();
	mHandle  = other.mHandle;
	mShaders = std::move(other.mShaders);
	other.release();
	return (*this);
}

void ShaderProgram::addShader(Shader &&shader)
{	
	glAttachShader(mHandle, shader.handle());
	mShaders.push_back(std::move(shader));
}

void ShaderProgram::link()
{
	bindAttributeLocations();

	glLinkProgram(mHandle);
	checkProgramError(GL_LINK_STATUS);

	glValidateProgram(mHandle);
	checkProgramError(GL_VALIDATE_STATUS);
}

void ShaderProgram::use()
{
	glUseProgram(mHandle);
}

void ShaderProgram::setUniform(const std::string &uniform, float value)
{
	glUniform1f(getUniformLocation(uniform), value);
}

void ShaderProgram::setUniform(const std::string &uniform, const core::vec2 &value)
{
	glUniform2f(getUniformLocation(uniform), value.x, value.y);
}

void ShaderProgram::setUniform(const std::string &uniform, const core::vec3 &value)
{
	glUniform3f(getUniformLocation(uniform), value.x, value.y, value.z);
}

void ShaderProgram::setUniform(const std::string &uniform, const core::vec4 &value)
{
	glUniform4f(getUniformLocation(uniform), value.x, value.y, value.z, value.w);
}

void ShaderProgram::setUniform(const std::string &uniform, const core::mat4 &value)
{
	glUniformMatrix4fv(getUniformLocation(uniform), 1, GL_TRUE, &value(0,0));
}

void ShaderProgram::setUniform(const std::string &uniform, const Material &material)
{
	setUniform(uniform + ".texture",   material.texture().bind(0));
	setUniform(uniform + ".ambient",   material.ambient());
	setUniform(uniform + ".diffuse",   material.diffuse());
	setUniform(uniform + ".specular",  material.specular());
	setUniform(uniform + ".shininess", material.shininess());
}



void ShaderProgram::bindAttributeLocations()
{
	glBindAttribLocation(mHandle, Mesh::POSITION, "position");
	glBindAttribLocation(mHandle, Mesh::TEXCOORD, "texcoord");
	glBindAttribLocation(mHandle, Mesh::NORMAL,   "normal");
}

GLint ShaderProgram::getUniformLocation(const std::string &uniform) const
{
	GLint location = glGetUniformLocation(mHandle, uniform.c_str());

	if (location == -1)
		throw std::runtime_error("could not get uniform location for '" + uniform + "'");

	return location;
}

GLint ShaderProgram::getAttributeLocation(const std::string &attribute) const
{
	GLint result = glGetAttribLocation(mHandle, attribute.c_str());

	if (result == -1)
		throw std::runtime_error("could not get attribute location for attribute '" + attribute + "'");

	return result;
}

void ShaderProgram::checkProgramError(GLenum linkingStage)
{
	GLint success;

	glGetProgramiv(mHandle, linkingStage, &success);

	if (!success)
	{
		char errorMessage[1024];
		glGetProgramInfoLog(mHandle, sizeof(errorMessage), NULL, errorMessage);

		throw std::runtime_error("shader program error:\n" + std::string(errorMessage));
	}

}

void ShaderProgram::release()
{
	mHandle = 0;
}

void ShaderProgram::destroy()
{
	if (mHandle != 0)
	{
		for (Shader &shader : mShaders)
			glDetachShader(mHandle, shader.handle());
		glDeleteProgram(mHandle);
	}
}

}}}
