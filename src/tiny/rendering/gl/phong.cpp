#include <tiny/core/camera.h>

#include "phong.h"
#include "material.h"
#include "mesh.h"

namespace tiny { namespace rendering { namespace gl {

PhongShader::PhongShader() : ShaderProgram(ShaderProgram::fromFiles("res/shaders/phong.vs", "res/shaders/phong.fs")) {}


void PhongShader::setUniform(const std::string &uniform, const BaseLight &lightSource)
{
	setUniform(uniform + ".color",     lightSource.color);
	setUniform(uniform + ".intensity", lightSource.intensity);
}

void PhongShader::setUniform(const std::string &uniform, const DirectionalLight &directionalLight)
{
	setUniform(uniform + ".base",     dynamic_cast<const BaseLight&>(directionalLight));
	setUniform(uniform + ".direction", directionalLight.direction);
}

void PhongShader::setUniform(const std::string &uniform, const Material &material)
{
	const unsigned int ambientTextureSlot = 1;

	setUniform(uniform + ".texture", ambientTextureSlot);
	setUniform(uniform + ".ambient", material.ambient());
	setUniform(uniform + ".diffuse", material.diffuse());
	setUniform(uniform + ".specular", material.specular());
	setUniform(uniform + ".shininess", material.shininess());

	material.texture().bind(ambientTextureSlot);
}

void PhongShader::setDirectionalLight(const DirectionalLight directional)
{
	mDirectional = directional;
}

void PhongShader::draw(const core::Camera &camera, const Mesh &mesh)
{
	use();
	setUniform("M",   mesh.modelMatrix());
	setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
	setUniform("material", mesh.material());
	setUniform("directionalLight", mDirectional);
	setUniform("eyePos", camera.position());

	mesh.draw();
}

}}}
