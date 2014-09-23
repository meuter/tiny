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
			mLightShader = ShaderProgram::fromFiles("res/shaders/light.vs", "res/shaders/light.fs");
		}

		void render(const core::Camera &camera, const Mesh &mesh)
		{
			mLightShader.use();
			mLightShader.setUniform("M",   mesh.modelMatrix());
			mLightShader.setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
			mLightShader.setUniform("material", mesh.material());
			mLightShader.setUniform("eyePosition", camera.position());
			mLightShader.setUniform("ambientLight", mAmbientLight);

			for (size_t i = 0; i < 10; i++)
				mLightShader.setUniform("lightSources["+std::to_string(i)+"].intensity", 0);

			for (size_t i = 0; i < mLightSources.size(); i++)
				mLightShader.setUniform("lightSources["+std::to_string(i)+"]", mLightSources[i]);

			mesh.draw();
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
			LightSource lightSource;

			lightSource.mColor = color;
			lightSource.mIntensity = intensity;
			lightSource.mPosition = position;
			lightSource.mDirection = normalize(direction);
			lightSource.mAttenuation = vec3(1,0,0);
			lightSource.mCutoff = cutoff;
			lightSource.mCutoffExponent = cutoffExponent;

			mLightSources.push_back(lightSource);
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

	private:
		ShaderProgram mAmbientLightShader, mLightShader;
		std::vector<LightSource> mLightSources;
		LightSource mAmbientLight;
		Context &mContext;
	};

}}}

#endif