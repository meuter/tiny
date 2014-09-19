#ifndef __TINY_RENDERING_GL_H__
#define __TINY_RENDERING_GL_H__

#include <tiny/core/camera.h>

#include "shaderprogram.h"
#include "mesh.h"

namespace tiny { namespace rendering { namespace gl {

	struct BaseLight
	{
		BaseLight() : color(0,0,0), intensity(0) {}
		BaseLight(const core::vec3 &color, const float &intensity) : color(color), intensity(intensity) {}
		core::vec3 color;
		float intensity;
	};

	struct DirectionalLight : public BaseLight
	{
		DirectionalLight() : BaseLight(), direction(0,0,0) {}
		DirectionalLight(const core::vec3 &color, const float &intensity, const core::vec3 &direction) : BaseLight(color, intensity), direction(direction) {}
		core::vec3 direction;
	};

	class DirectionalLightShader : public ShaderProgram
	{
	public:	
		using ShaderProgram::setUniform;
		using ShaderProgram::use;

		DirectionalLightShader() : ShaderProgram(ShaderProgram::fromFiles("res/shaders/directional.vs", "res/shaders/directional.fs")) {}

		void setUniform(const std::string &uniform, const BaseLight &lightSource)
		{
			setUniform(uniform + ".color",     lightSource.color);
			setUniform(uniform + ".intensity", lightSource.intensity);
		}

		void setUniform(const std::string &uniform, const DirectionalLight &directionalLight)
		{
			setUniform(uniform + ".base",     dynamic_cast<const BaseLight&>(directionalLight));
			setUniform(uniform + ".direction", directionalLight.direction);
		}

		void shade(const core::Camera &camera, const Mesh &mesh, const DirectionalLight &directionalLight)
		{
			use();
			setUniform("M",   mesh.modelMatrix());
			setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
			setUniform("material", mesh.material());
			setUniform("directionalLight", directionalLight);
			setUniform("eyePos", camera.position());

			mesh.draw();
		}
	};

}}}

#endif

