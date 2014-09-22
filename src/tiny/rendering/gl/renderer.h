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
			mAmbientShader     = ShaderProgram::fromFiles("res/shaders/ambient.vs", "res/shaders/ambient.fs");
			mDirectionalShader = ShaderProgram::fromFiles("res/shaders/directional.vs", "res/shaders/directional.fs");
			mPointShader       = ShaderProgram::fromFiles("res/shaders/point.vs", "res/shaders/point.fs");
		}

		void ambientPass(const core::Camera &camera, const Mesh &mesh)
		{
			mAmbientShader.use();
			mAmbientShader.setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
			mAmbientShader.setUniform("material", mesh.material());
			mesh.draw();
		}

		void directionalLightPass(const core::Camera &camera, const Mesh &mesh, const DirectionalLight &directionalLight)
		{
			mDirectionalShader.use();
			mDirectionalShader.setUniform("M",   mesh.modelMatrix());
			mDirectionalShader.setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
			mDirectionalShader.setUniform("material", mesh.material());
			mDirectionalShader.setUniform("directionalLight", directionalLight);
			mDirectionalShader.setUniform("eyePos", camera.position());
			mesh.draw();
		}

		void pointLightPass(const core::Camera &camera, const Mesh &mesh, const PointLight &pointLight)
		{
			mPointShader.use();
			mPointShader.setUniform("M",   mesh.modelMatrix());
			mPointShader.setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
			mPointShader.setUniform("material", mesh.material());
			mPointShader.setUniform("pointLight", pointLight);
			mPointShader.setUniform("eyePos", camera.position());
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

		void render(const core::Camera &camera, const Mesh &mesh)
		{
			ambientPass(camera, mesh);		
			mContext.enableBlending();
			{
				for (const auto &directionalLight : mDirectionalLights)
					directionalLightPass(camera, mesh, directionalLight);

				for (const auto &pointLight : mPointLights)
					pointLightPass(camera, mesh, pointLight);
			}
			mContext.disableBlending();
		}

	private:
		ShaderProgram mAmbientShader, mDirectionalShader, mPointShader;
		std::vector<DirectionalLight> mDirectionalLights;
		std::vector<PointLight> mPointLights;
		Context &mContext;
	};

}}}

#endif