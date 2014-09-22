#include <stdexcept>
#include <fstream>
#include <sstream>
#include <utility>

#include "shaderprogram.h"
#include "mesh.h"
#include "material.h"
#include "light.h"

namespace tiny { namespace rendering { namespace gl {

ShaderProgram ShaderProgram::fromFiles(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename)
{
	ShaderProgram result;
	result.add(Shader::fromFile(GL_VERTEX_SHADER,   vertexShaderFilename));
	result.add(Shader::fromFile(GL_FRAGMENT_SHADER, fragmentShaderFilename));
	result.link();
	return result;
}

ShaderProgram::ShaderProgram() : mHandle(glCreateProgram())
{
	if (mHandle == 0)
		throw std::runtime_error("could not create shader program");
}

void ShaderProgram::add(Shader &&shader)
{	
	glAttachShader(mHandle, shader.handle());
}

void ShaderProgram::link()
{
	bindAttributeLocations();

	glLinkProgram(mHandle);
	check(GL_LINK_STATUS);

	glValidateProgram(mHandle);
	check(GL_VALIDATE_STATUS);
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
	material.texture().bind(0);

	setUniform(uniform + ".texture",   0);
	setUniform(uniform + ".ambient",   material.ambient());
	setUniform(uniform + ".diffuse",   material.diffuse());
	setUniform(uniform + ".specular",  material.specular());
	setUniform(uniform + ".shininess", material.shininess());
}

void ShaderProgram::setUniform(const std::string &uniform, const AmbientLight &ambientLight)
{
	setUniform(uniform + ".color",     ambientLight.color());
	setUniform(uniform + ".intensity", ambientLight.intensity());
}

void ShaderProgram::setUniform(const std::string &uniform, const DirectionalLight &directionalLight)
{
	setUniform(uniform, dynamic_cast<const AmbientLight&>(directionalLight));
	setUniform(uniform + ".direction", directionalLight.direction());
}

void ShaderProgram::setUniform(const std::string &uniform, const PointLight &pointLight)
{
	setUniform(uniform, dynamic_cast<const AmbientLight&>(pointLight));
	setUniform(uniform + ".position",    pointLight.position());
	setUniform(uniform + ".attenuation", pointLight.attenuation());
	setUniform(uniform + ".range",       pointLight.range());
}

void ShaderProgram::setUniform(const std::string &uniform, const SpotLight &spotLight)
{
	setUniform(uniform, dynamic_cast<const PointLight&>(spotLight));
	setUniform(uniform + ".direction", spotLight.direction());
	setUniform(uniform + ".cutoff",    spotLight.cutoff());
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

void ShaderProgram::check(GLenum linkingStage)
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

void ShaderProgram::destroy(GLuint handle)
{
	glDeleteProgram(handle);
}

}}}
