#ifndef __TINY_RENDERING_GL_POINT_H__
#define __TINY_RENDERING_GL_POINT_H__

#include <tiny/core/camera.h>

#include "shaderprogram.h"
#include "mesh.h"

namespace tiny { namespace rendering { namespace gl {

	struct PointLight
	{
		PointLight() : color(1,1,1), intensity(0), position(0,0,0), attenuation(0,1,0) {}
		PointLight(const core::vec3 &color, const float &intensity, const core::vec3 &position, const core::vec3 attenuation) 
			: color(color), intensity(intensity), position(position), attenuation(attenuation) {}

		core::vec3 color;
		float intensity;
		core::vec3 position;
		core::vec3 attenuation;
	};

	class PointLightShader : public ShaderProgram
	{
	public:	
		using ShaderProgram::setUniform;
		using ShaderProgram::use;

		PointLightShader() : ShaderProgram(ShaderProgram::fromFiles("res/shaders/point.vs", "res/shaders/point.fs")) {}

		void setUniform(const std::string &uniform, const PointLight &pointLight)
		{
			setUniform(uniform + ".color",       pointLight.color);
			setUniform(uniform + ".intensity",   pointLight.intensity);
			setUniform(uniform + ".position",    pointLight.position);
			setUniform(uniform + ".attenuation", pointLight.attenuation);
		}

		void shade(const core::Camera &camera, const Mesh &mesh, const PointLight &pointLight)
		{
			use();
			setUniform("M",   mesh.modelMatrix());
			setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
			setUniform("material", mesh.material());
			setUniform("pointLight", pointLight);
			setUniform("eyePos", camera.position());

			mesh.draw();
		}
	};

}}}

#endif

