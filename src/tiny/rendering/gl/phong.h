#ifndef __TINY_RENDERING_GL_PHONG_H__
#define __TINY_RENDERING_GL_PHONG_H__

#include <tiny/core/types.h>
#include "shaderprogram.h"

namespace tiny { namespace core {
	class Camera;
}}

namespace tiny { namespace rendering {
	class Mesh;
}}

namespace tiny { namespace rendering { namespace gl {
	
	class Material;

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

	class PhongShader : public ShaderProgram
	{
	public:
		using ShaderProgram::setUniform;
		using ShaderProgram::use;

		PhongShader();

		void setUniform(const std::string &uniform, const BaseLight &lightSource);
		void setUniform(const std::string &uniform, const DirectionalLight &directionalLight);
		void setUniform(const std::string &uniform, const Material &material);

		void setDirectionalLight(const DirectionalLight directional);

		void draw(const core::Camera &camera, const Mesh &mesh);
	private:
		DirectionalLight mDirectional;
	};

}}}

#endif