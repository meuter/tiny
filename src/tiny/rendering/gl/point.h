#ifndef __TINY_RENDERING_GL_POINT_H__
#define __TINY_RENDERING_GL_POINT_H__

#include <tiny/core/camera.h>
#include <algorithm>

#include <iostream>

#include "shaderprogram.h"
#include "mesh.h"

namespace tiny { namespace rendering { namespace gl {

	struct PointLight
	{
		PointLight() : color(1,1,1), intensity(0), position(0,0,0), attenuation(1,0,0) 
		{
			computeRange();
		}

		PointLight(const core::vec3 &color, float intensity, const core::vec3 &position, const core::vec3 attenuation = core::vec3(1,0,0))
			: color(color), intensity(intensity), position(position), attenuation(attenuation)
		{
			computeRange();
		}

		void computeRange()
		{			
			float a = attenuation.x;
			float b = attenuation.y;
			float c = attenuation.z - 256 * intensity * std::max(std::max(color.x, color.y), color.z);

			float delta = b*b - 4*a-c;

			range  = (-b + sqrt(delta)) / 2*a;

			std::cout << "range = " << range << std::endl;

			// range = 100;
		}

		core::vec3 color;
		float intensity;
		core::vec3 position;
		core::vec3 attenuation;	
		float range;
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
			setUniform(uniform + ".range",       pointLight.range);
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

