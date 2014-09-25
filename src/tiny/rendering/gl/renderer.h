#ifndef __TINY_RENDERING_GL_PHONG_H__
#define __TINY_RENDERING_GL_PHONG_H__

#include <tiny/core/types.h>
#include <tiny/core/camera.h>

#include "shaderprogram.h"
#include "material.h"
#include "mesh.h"
#include "context.h"
#include "../lightsource.h"
#include "scene.h"

namespace tiny { namespace rendering { namespace gl {

	class Renderer
	{
		using vec3 = core::vec3;
	public:	

		Renderer(Context &context) : mContext(context) {}
		
		void init()
		{
			mContext.enableBackfaceCulling();
			mContext.enableDepthTest();
		}

		void render(const Scene &scene)
		{
			size_t nLights = scene.lightSourceCount(), currentLight = 0;

			ShaderProgram &shader = getShader(nLights);

			shader.use();
			shader.setUniform("ambientLight", scene.ambientLight());

			if (nLights > 0) 
				shader.setUniform("eyePosition", scene.camera().position());

			scene.forAll([&](const LightSource &light) 
			{
			 	shader.setUniform("lightSources["+std::to_string(currentLight++)+"]", light);
			});

			scene.forAll([&](const Mesh &mesh)
			{
			 	shader.setUniform("M",   mesh.modelMatrix());
			 	shader.setUniform("MVP", scene.camera().projectionMatrix() * scene.camera().viewMatrix() * mesh.modelMatrix());
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
	};

}}}

#endif