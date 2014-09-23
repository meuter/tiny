#ifndef __TINY_RENDERING_GL_PHONG_H__
#define __TINY_RENDERING_GL_PHONG_H__

#include <tiny/core/types.h>
#include <tiny/core/camera.h>

#include "shaderprogram.h"
#include "material.h"
#include "mesh.h"
#include "context.h"
#include "light.h"

namespace tiny { namespace rendering { namespace gl {

	class Renderer
	{
	public:	

		Renderer(Context &context) : mContext(context) 
		{
			mAmbientLightShader     = ShaderProgram::fromFiles("res/shaders/ambient.vs",     "res/shaders/ambient.fs");
			mDirectionalLightShader = ShaderProgram::fromFiles("res/shaders/directional.vs", "res/shaders/directional.fs");
			mPointLightShader       = ShaderProgram::fromFiles("res/shaders/point.vs",       "res/shaders/point.fs");
			mSpotLightShader        = ShaderProgram::fromFiles("res/shaders/spot.vs",        "res/shaders/spot.fs");
		}

		void ambientPass(const core::Camera &camera, const Mesh &mesh, const AmbientLight ambientLight)
		{
			mAmbientLightShader.use();
			mAmbientLightShader.setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
			mAmbientLightShader.setUniform("material", mesh.material());
			mAmbientLightShader.setUniform("ambientLight", ambientLight);
			mesh.draw();
		}

		void directionalLightPass(const core::Camera &camera, const Mesh &mesh, const DirectionalLight &directionalLight)
		{
			mDirectionalLightShader.use();
			mDirectionalLightShader.setUniform("M",   mesh.modelMatrix());
			mDirectionalLightShader.setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
			mDirectionalLightShader.setUniform("material", mesh.material());
			mDirectionalLightShader.setUniform("directionalLight", directionalLight);
			mDirectionalLightShader.setUniform("eyePos", camera.position());
			mesh.draw();
		}

		void pointLightPass(const core::Camera &camera, const Mesh &mesh, const PointLight &pointLight)
		{
			mPointLightShader.use();
			mPointLightShader.setUniform("M",   mesh.modelMatrix());
			mPointLightShader.setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
			mPointLightShader.setUniform("material", mesh.material());
			mPointLightShader.setUniform("pointLight", pointLight);
			mPointLightShader.setUniform("eyePos", camera.position());
			mesh.draw();
		}


		void spotLightPass(const core::Camera &camera, const Mesh &mesh, const SpotLight &spotLight)
		{
			mSpotLightShader.use();
			mSpotLightShader.setUniform("M",   mesh.modelMatrix());
			mSpotLightShader.setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
			mSpotLightShader.setUniform("material", mesh.material());
			mSpotLightShader.setUniform("spotLight", spotLight);
			mSpotLightShader.setUniform("eyePos", camera.position());
			mesh.draw();
		}

		void init()
		{
			mContext.enableBackfaceCulling();
			mContext.enableDepthTest();
		}

		void addDirectionalLight(const DirectionalLight &directionalLight)
		{
			mDirectionalLights.push_back(directionalLight);
		}

		void addPointLight(const PointLight &pointLight)
		{
			mPointLights.push_back(pointLight);
		}

		void addSpotLight(const SpotLight &spotLight)
		{
			mSpotLights.push_back(spotLight);
		}

		void setAmbientLight(const AmbientLight &ambientLight)
		{
			mAmbientLight = ambientLight;
		}

		void render(const core::Camera &camera, const Mesh &mesh)
		{
			ambientPass(camera, mesh, mAmbientLight);		
			mContext.enableBlending();
			{
				for (const auto &directionalLight : mDirectionalLights)
					directionalLightPass(camera, mesh, directionalLight);

				for (const auto &pointLight : mPointLights)
					pointLightPass(camera, mesh, pointLight);

				for (const auto &spotLight : mSpotLights)
					spotLightPass(camera, mesh, spotLight);
			}
			mContext.disableBlending();
		}

	private:
		ShaderProgram mAmbientLightShader, mDirectionalLightShader, mPointLightShader, mSpotLightShader;
		std::vector<DirectionalLight> mDirectionalLights;
		std::vector<PointLight> mPointLights;
		std::vector<SpotLight> mSpotLights;
		AmbientLight mAmbientLight;
		Context &mContext;
	};

}}}

#endif