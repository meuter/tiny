#include "shaderprogram.h"
#include "texture.h"
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
	: mProgramHandle(other.mProgramHandle), mShaders(std::move(other.mShaders)), mUniformLocations(std::move(other.mUniformLocations))
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
	mUniformLocations = std::move(other.mUniformLocations);
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
	glLinkProgram(mProgramHandle);
	checkProgramError(GL_LINK_STATUS);

	glValidateProgram(mProgramHandle);
	checkProgramError(GL_VALIDATE_STATUS);
}

void ShaderProgram::use()
{
	glUseProgram(mProgramHandle);
}

void ShaderProgram::detectUniform(const std::string &uniform)
{
	GLint location = glGetUniformLocation(mProgramHandle, uniform.c_str());

	if (location == -1)
		throw std::runtime_error("could not get uniform location for '" + uniform + "'");

	mUniformLocations[uniform] = location;
}

void ShaderProgram::setUniform(const std::string &uniform, float value)
{
	glUniform1f(mUniformLocations[uniform], value);
}

void ShaderProgram::setUniform(const std::string &uniform, const core::vec2 &value)
{
	glUniform2f(mUniformLocations[uniform], value.x, value.y);
}

void ShaderProgram::setUniform(const std::string &uniform, const core::vec3 &value)
{
	glUniform3f(mUniformLocations[uniform], value.x, value.y, value.z);
}

void ShaderProgram::setUniform(const std::string &uniform, const core::vec4 &value)
{
	glUniform4f(mUniformLocations[uniform], value.x, value.y, value.z, value.w);
}

void ShaderProgram::setUniform(const std::string &uniform, const core::mat4 &value)
{
	glUniformMatrix4fv(mUniformLocations[uniform], 1, GL_TRUE, &value(0,0));
}

GLint ShaderProgram::getAttributeLocation(const std::string &attribute) const
{
	GLint result = glGetAttribLocation(mProgramHandle, attribute.c_str());

	if (result == -1)
		throw std::runtime_error("could not get attribute location for attribyte '" + attribute + "'");

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
