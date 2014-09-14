#include "shaderprogram.h"
#include "texture.h"
#include "mesh.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <utility>


namespace tiny { namespace rendering {

ShaderProgram ShaderProgram::fromFiles(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename)
{
	auto result = ShaderProgram();
	result.addShader(Shader::fromFile(GL_VERTEX_SHADER,   vertexShaderFilename));
	result.addShader(Shader::fromFile(GL_FRAGMENT_SHADER, fragmentShaderFilename));
	result.link();
	return result;
}

ShaderProgram::ShaderProgram() : mProgramHandle(glCreateProgram())
{
	if (mProgramHandle == 0)
		throw std::runtime_error("could not create program");
}

ShaderProgram::ShaderProgram(ShaderProgram &&other) 
	: mProgramHandle(other.mProgramHandle), mShaders(std::move(other.mShaders))
{
	other.mProgramHandle = 0;
}

ShaderProgram::~ShaderProgram()
{
	destroy();
}

ShaderProgram &ShaderProgram::operator=(ShaderProgram &&other)
{
	destroy();
	mProgramHandle    = other.mProgramHandle;
	mShaders          = std::move(other.mShaders);
	other.mProgramHandle = 0;
	return (*this);
}

void ShaderProgram::addShader(Shader &&shader)
{	
	glAttachShader(mProgramHandle, shader.getHandle());
	mShaders.push_back(std::move(shader));
}

void ShaderProgram::link()
{
	bindAttributeLocations();

	glLinkProgram(mProgramHandle);
	checkProgramError(GL_LINK_STATUS);

	glValidateProgram(mProgramHandle);
	checkProgramError(GL_VALIDATE_STATUS);
}

void ShaderProgram::use()
{
	glUseProgram(mProgramHandle);
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

void ShaderProgram::bindAttributeLocations()
{
	glBindAttribLocation(mProgramHandle, Mesh::POSITION, "position");
	glBindAttribLocation(mProgramHandle, Mesh::TEXCOORD, "texcoord");
}

GLint ShaderProgram::getUniformLocation(const std::string &uniform)
{
	GLint location = glGetUniformLocation(mProgramHandle, uniform.c_str());

	if (location == -1)
		throw std::runtime_error("could not get uniform location for '" + uniform + "'");

	return location;
}

GLint ShaderProgram::getAttributeLocation(const std::string &attribute) const
{
	GLint result = glGetAttribLocation(mProgramHandle, attribute.c_str());

	if (result == -1)
		throw std::runtime_error("could not get attribute location for attribute '" + attribute + "'");

	return result;
}

void ShaderProgram::checkProgramError(GLenum linkingStage)
{
	GLint success;

	glGetProgramiv(mProgramHandle, linkingStage, &success);

	if (!success)
	{
		char errorMessage[1024];
		glGetProgramInfoLog(mProgramHandle, sizeof(errorMessage), NULL, errorMessage);

		throw std::runtime_error("shader program error:\n" + std::string(errorMessage));
	}

}

void ShaderProgram::destroy()
{
	if (mProgramHandle != 0)
	{
		for (Shader &shader : mShaders)
			glDetachShader(mProgramHandle, shader.getHandle());
		glDeleteProgram(mProgramHandle);
	}
}

}}
