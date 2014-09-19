#ifndef __TINY_RENDERING_GL_DIRECTIONAL_H__
#define __TINY_RENDERING_GL_DIRECTIONAL_H__

#include <tiny/core/camera.h>

#include "shaderprogram.h"
#include "mesh.h"

namespace tiny { namespace rendering { namespace gl {

	struct DirectionalLight
	{
		DirectionalLight() : color(1,1,1), intensity(0), direction(0,0,0) {}
		DirectionalLight(const core::vec3 &color, const float &intensity, const core::vec3 &direction) 
			: color(color), intensity(intensity), direction(direction) {}

		core::vec3 color;
		float intensity;
		core::vec3 direction;
	};

	class DirectionalLightShader : public ShaderProgram
	{
	public:	
		using ShaderProgram::setUniform;
		using ShaderProgram::use;

		DirectionalLightShader() : ShaderProgram(ShaderProgram::fromFiles("res/shaders/directional.vs", "res/shaders/directional.fs")) {}

		void setUniform(const std::string &uniform, const DirectionalLight &directionalLight)
		{
			setUniform(uniform + ".color",     directionalLight.color);
			setUniform(uniform + ".intensity", directionalLight.intensity);
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

