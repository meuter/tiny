#include "renderer.h"

namespace tiny { namespace rendering { namespace gl {

	Renderer::Renderer(Context &context, Scene &scene) : mContext(context), mScene(scene) {}
	
	void Renderer::init()
	{
		mContext.enableBackfaceCulling();
		mContext.enableDepthTest();
	}

	void Renderer::update(Game &game, sec t, sec dt)	{}

	void Renderer::render()
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

	ShaderProgram &Renderer::getShader(int lightSourceCount)
	{
		if (mLightShaders.find(lightSourceCount) == mLightShaders.end())
		{
			ShaderProgram::Constants constants;

			constants["MAX_LIGHT_SOURCES"] = std::to_string(lightSourceCount);
			mLightShaders[lightSourceCount] = ShaderProgram::fromFiles("res/shaders/light.vs", "res/shaders/light.fs", constants);
		}

		return mLightShaders[lightSourceCount];
	}


}}}
