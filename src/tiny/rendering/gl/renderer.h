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
			mLightShader = ShaderProgram::fromFiles("res/shaders/light.vs", "res/shaders/light.fs");
		}

		void ambientPass(const core::Camera &camera, const Mesh &mesh)
		{
			mAmbientLightShader.use();
			mAmbientLightShader.setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
			mAmbientLightShader.setUniform("material", mesh.material());
			mAmbientLightShader.setUniform("ambientLight", mAmbientLight);
			mesh.draw();
		}

		void spotLightPass(const core::Camera &camera, const Mesh &mesh)
		{
			mLightShader.use();
			mLightShader.setUniform("M",   mesh.modelMatrix());
			mLightShader.setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
			mLightShader.setUniform("material", mesh.material());
			mLightShader.setUniform("eyePos", camera.position());

			for (const auto &spotLight : mLightSources)
			{
				mLightShader.setUniform("spotLight", spotLight);
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

			mLightSources.push_back(directionalLight);
		}

		void addPointLight(const vec3 &color, float intensity, const vec3 &position)
		{
			LightSource pointLight;

			pointLight.mColor = color;
			pointLight.mIntensity = intensity;
			pointLight.mPosition = position;
			pointLight.mDirection = vec3(0,0,0);
			pointLight.mAttenuation = vec3(1,0,0);
			pointLight.mCutoff = -1.0f;
			pointLight.mCutoffExponent = 0.0f;

			mLightSources.push_back(pointLight);
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

			mLightSources.push_back(spotLight);
		}

		void setAmbientLight(const vec3 &color, float intensity)
		{
			mAmbientLight.mColor = color;
			mAmbientLight.mDirection = vec3(0,0,0);
			mAmbientLight.mPosition = vec3(0,0,0);
			mAmbientLight.mAttenuation = vec3(0,0,1);
			mAmbientLight.mIntensity = intensity;
			mAmbientLight.mCutoff = -1.0f;
			mAmbientLight.mCutoffExponent = 0.0f;
		}

		void render(const core::Camera &camera, const Mesh &mesh)
		{
			ambientPass(camera, mesh);		
			mContext.enableBlending();
			spotLightPass(camera, mesh);
			mContext.disableBlending();
		}

	private:
		ShaderProgram mAmbientLightShader, mLightShader;
		std::vector<LightSource> mLightSources;
		LightSource mAmbientLight;
		Context &mContext;
	};

}}}

#endif