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
		using vec3 = core::vec3;
	public:	

		Renderer(Context &context) : mContext(context) 
		{
			mAmbientLightShader     = ShaderProgram::fromFiles("res/shaders/light.vs", "res/shaders/ambient.fs");
			mDirectionalLightShader = ShaderProgram::fromFiles("res/shaders/light.vs", "res/shaders/directional.fs");
			mPointLightShader       = ShaderProgram::fromFiles("res/shaders/light.vs", "res/shaders/point.fs");
			mSpotLightShader        = ShaderProgram::fromFiles("res/shaders/light.vs", "res/shaders/spot.fs");
		}

		void ambientPass(const core::Camera &camera, const Mesh &mesh)
		{
			mAmbientLightShader.use();
			mAmbientLightShader.setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
			mAmbientLightShader.setUniform("material", mesh.material());
			mAmbientLightShader.setUniform("ambientLight", mAmbientLight);
			mesh.draw();
		}

		void directionalLightPass(const core::Camera &camera, const Mesh &mesh)
		{
			mDirectionalLightShader.use();
			mDirectionalLightShader.setUniform("M",   mesh.modelMatrix());
			mDirectionalLightShader.setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
			mDirectionalLightShader.setUniform("material", mesh.material());
			mDirectionalLightShader.setUniform("eyePos", camera.position());

			for (const auto &directionalLight: mDirectionalLights)
			{
				mDirectionalLightShader.setUniform("directionalLight", directionalLight);
				mesh.draw();
			}
		}

		void pointLightPass(const core::Camera &camera, const Mesh &mesh)
		{
			mPointLightShader.use();
			mPointLightShader.setUniform("M",   mesh.modelMatrix());
			mPointLightShader.setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
			mPointLightShader.setUniform("material", mesh.material());
			mPointLightShader.setUniform("eyePos", camera.position());

			for (const auto &pointLight : mPointLights)
			{
				mPointLightShader.setUniform("pointLight", pointLight);
				mesh.draw();
			}
		}


		void spotLightPass(const core::Camera &camera, const Mesh &mesh)
		{
			mSpotLightShader.use();
			mSpotLightShader.setUniform("M",   mesh.modelMatrix());
			mSpotLightShader.setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
			mSpotLightShader.setUniform("material", mesh.material());
			mSpotLightShader.setUniform("eyePos", camera.position());

			for (const auto &spotLight : mSpotLights)
			{
				mSpotLightShader.setUniform("spotLight", spotLight);
				mesh.draw();
			}
		}

		void init()
		{
			mContext.enableBackfaceCulling();
			mContext.enableDepthTest();
		}

		void addDirectionalLight(const vec3 &color, float intensity, const vec3 &direction)		
		{
			LightSource directionalLight;

			directionalLight.mColor = color;
			directionalLight.mDirection = normalize(direction);
			directionalLight.mPosition = -normalize(direction) * 1.5e11f;
			directionalLight.mAttenuation = vec3(0,0,1);
			directionalLight.mIntensity = intensity;
			directionalLight.mCutoff = -1.0f;
			directionalLight.mCutoffExponent = 0.0f;

			mDirectionalLights.push_back(directionalLight);
		}

		void addPointLight(const vec3 &color, float intensity, const vec3 &position)
		{
			LightSource pointLight;

			pointLight.mColor = color;
			pointLight.mIntensity = intensity;
			pointLight.mPosition = position;
			pointLight.mDirection = vec3(0,0,0);
			pointLight.mAttenuation = vec3(1,0,0);

			mPointLights.push_back(pointLight);
		}

		void addSpotLight(const vec3 &color, float intensity, const vec3 &position, const vec3 &direction, float cutoff, float cutoffExponent)
		{
			LightSource spotLight;

			spotLight.mColor = color;
			spotLight.mIntensity = intensity;
			spotLight.mPosition = position;
			spotLight.mDirection = normalize(direction);
			spotLight.mAttenuation = vec3(1,0,0);
			spotLight.mCutoff = cutoff;
			spotLight.mCutoffExponent = cutoffExponent;

			mSpotLights.push_back(spotLight);
		}

		void setAmbientLight(const vec3 &color, float intensity)
		{
			mAmbientLight.mColor = color;
			mAmbientLight.mIntensity = intensity;
		}

		void render(const core::Camera &camera, const Mesh &mesh)
		{
			ambientPass(camera, mesh);		
			mContext.enableBlending();
			{
				directionalLightPass(camera, mesh);
				pointLightPass(camera, mesh);
				spotLightPass(camera, mesh);
			}
			mContext.disableBlending();
		}

	private:
		ShaderProgram mAmbientLightShader, mDirectionalLightShader, mPointLightShader, mSpotLightShader;
		std::vector<LightSource> mDirectionalLights, mPointLights, mSpotLights;
		LightSource mAmbientLight;
		Context &mContext;
	};

}}}

#endif