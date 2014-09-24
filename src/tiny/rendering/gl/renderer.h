#ifndef __TINY_RENDERING_GL_PHONG_H__
#define __TINY_RENDERING_GL_PHONG_H__

#include <tiny/core/types.h>
#include <tiny/core/camera.h>

#include "shaderprogram.h"
#include "material.h"
#include "mesh.h"
#include "context.h"
#include "../lightsource.h"

namespace tiny { namespace rendering { namespace gl {

	class Renderer
	{
		using vec3 = core::vec3;
	public:	

		Renderer(Context &context) : mContext(context) {}
		
		void render(const core::Camera &camera, const Mesh &mesh)
		{
			ShaderProgram &shader = getShader(mLightSources.size());

			shader.use();
			shader.setUniform("M",   mesh.modelMatrix());
			shader.setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
			shader.setUniform("material", mesh.material());
			shader.setUniform("ambientLight", mAmbientLight);

			if (mLightSources.size() > 0)
				shader.setUniform("eyePosition", camera.position());

			for (size_t i = 0; i < mLightSources.size(); i++)
				shader.setUniform("lightSources["+std::to_string(i)+"]", mLightSources[i]);

			mesh.draw();
		}

		ShaderProgram &getShader(int lightSourceCount)
		{
			if (mLightShaders.find(lightSourceCount) == mLightShaders.end())
			{
				ShaderProgram::Constants constants;

				constants["MAX_LIGHT_SOURCES"] = std::to_string(lightSourceCount);
				mLightShaders[lightSourceCount] = ShaderProgram::fromFiles("res/shaders/light.vs", "res/shaders/light.fs", constants);
			}

			return mLightShaders[lightSourceCount];
		}

		void init()
		{
			mContext.enableBackfaceCulling();
			mContext.enableDepthTest();
		}

		void addDirectionalLight(const vec3 &color, float intensity, const vec3 &direction)		
		{
			mLightSources.push_back(LightSource::directional(color, intensity, direction));
		}

		void addPointLight(const vec3 &color, float intensity, const vec3 &position)
		{
			mLightSources.push_back(LightSource::point(color, intensity, position));
		}

		void addSpotLight(const vec3 &color, float intensity, const vec3 &position, const vec3 &direction, float cutoff, float cutoffExponent)
		{
			mLightSources.push_back(LightSource::spot(color, intensity, position, direction, cutoff, cutoffExponent));
		}

		void setAmbientLight(const vec3 &color, float intensity)
		{
			mAmbientLight = LightSource::ambient(color, intensity);
		}

	private:
		ShaderProgram mAmbientLightShader;
		std::map<int,ShaderProgram> mLightShaders;
		std::vector<LightSource> mLightSources;
		LightSource mAmbientLight;
		Context &mContext;
	};

}}}

#endif