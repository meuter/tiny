#ifndef __TINY_RENDERING_GL_PHONG_H__
#define __TINY_RENDERING_GL_PHONG_H__

#include <tiny/core/types.h>
#include <tiny/core/camera.h>

#include "shaderprogram.h"
#include "material.h"
#include "mesh.h"
#include "context.h"
#include "scene.h"

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


	class PhongShader : public ShaderProgram
	{
	public:
		using ShaderProgram::setUniform;
		using ShaderProgram::use;

		PhongShader() = default;
		PhongShader(ShaderProgram &&other) : ShaderProgram(std::move(other)) {}

		virtual ~PhongShader() = default;

		void setUniform(const std::string &uniform, const Material &material)
		{
			setUniform(uniform + ".texture",   material.texture().bind(0));
			setUniform(uniform + ".ambient",   material.ambient());
			setUniform(uniform + ".diffuse",   material.diffuse());
			setUniform(uniform + ".specular",  material.specular());
			setUniform(uniform + ".shininess", material.shininess());
		}
	};

	class AmbientShader : public PhongShader
	{
	public:
		using PhongShader::setUniform;
		using PhongShader::use;

		AmbientShader() : PhongShader(ShaderProgram::fromFiles("res/shaders/ambient.vs", "res/shaders/ambient.fs")) {}

		void shade(const core::Camera &camera, const Mesh &mesh)
		{
			use();
			setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
			setUniform("material", mesh.material());

			mesh.draw();
		}
	};


	class DirectionalLightShader : public PhongShader
	{
	public:	
		using PhongShader::setUniform;
		using PhongShader::use;

		DirectionalLightShader() : PhongShader(ShaderProgram::fromFiles("res/shaders/directional.vs", "res/shaders/directional.fs")) {}

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

	class PhongRenderer
	{
	public:	

		PhongRenderer(Context &context) : mContext(context) {}

		void init()
		{
			mContext.enableBackfaceCulling();
			mContext.enableDepthTest();
		}

		void addDirectionalLight(DirectionalLight &&directionalLight)
		{
			mDirectionalLights.push_back(std::move(directionalLight));
		}

		void render(const core::Camera &camera, const Mesh &mesh)
		{
			mAmbientShader.shade(camera, mesh);
			mContext.enableBlending();
			for (const auto &directionalLight : mDirectionalLights)
				mDirectionalShader.shade(camera, mesh, directionalLight);
			mContext.disableBlending();
		}

		void render(const core::Camera &camera, const Scene &scene)
		{
			for (const auto &mesh: scene.meshes())
				render(camera, mesh);
		}

	private:
		AmbientShader mAmbientShader;
		DirectionalLightShader mDirectionalShader;
		Context &mContext;
		std::vector<DirectionalLight> mDirectionalLights;
	};

}}}

#endif