#ifndef __TINY_RENDERING_GL_RENDERER_H__
#define __TINY_RENDERING_GL_RENDERER_H__

#include <tiny/core/types.h>
#include <tiny/core/camera.h>
#include <tiny/core/lightsource.h>

#include "shaderprogram.h"
#include "material.h"
#include "mesh.h"
#include "context.h"
#include "scene.h"

#include <tiny/core/game.h>

namespace tiny { namespace rendering { namespace gl {

	class Renderer 
	{
		using vec3 = core::vec3;
		using sec = core::sec;
		using Game = core::Game;
		using LightSource = core::LightSource;
	public:	

		Renderer(Context &context, Scene &scene) : mContext(context), mScene(scene) {}
		
		void init()
		{
			mContext.enableBackfaceCulling();
			mContext.enableDepthTest();
		}

		void update(Game &game, sec t, sec dt)	{}

		void render()
		{
			size_t nLights = mScene.lightSourceCount(), currentLight = 0;

			ShaderProgram &shader = getShader(nLights);

			shader.use();
			shader.setUniform("ambientLight", mScene.ambientLight());

			if (nLights > 0) 
				shader.setUniform("eyePosition", mScene.camera().position());

			mScene.forAll([&](const LightSource &light) 
			{
			 	shader.setUniform("lightSources["+std::to_string(currentLight++)+"]", light);
			});

			mScene.forAll([&](const Mesh &mesh)
			{
			 	shader.setUniform("M",   mesh.modelMatrix());
			 	shader.setUniform("MVP", mScene.camera().projectionMatrix() * mScene.camera().viewMatrix() * mesh.modelMatrix());
			 	shader.setUniform("material", mesh.material());
			 	mesh.draw();
			});
		}

	protected:	

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


	private:
		std::map<int,ShaderProgram> mLightShaders;
		Context &mContext;
		const Scene &mScene;
	};

}}}

#endif