#include "renderer.h"
#include "framebuffer.h"


namespace tiny { namespace rendering { namespace gl {

	// FIXME temp code to check render to texture
	core::Camera fakeCamera;
	std::unique_ptr<Mesh> screen1, screen2;
	std::unique_ptr<FrameBuffer> frameBuffer;
	core::ivec4 viewPort;

	Renderer::Renderer(Context &context, Scene &scene) : mContext(context), mScene(scene) {}
	
	void Renderer::init()
	{
	 	mContext.vsync(false);
		mContext.enableBackfaceCulling();
		mContext.enableDepthTest();

		// FIXME temp code to check render to texture
		{
			viewPort = mContext.getViewPort();

			frameBuffer.reset(new FrameBuffer(viewPort.z, viewPort.w, { GL_COLOR_ATTACHMENT0 } ) );

			fakeCamera.withPerspective(math::toRadian(70), viewPort.z/viewPort.w, 0.01f, 1000.0f)
				.moveTo(0,0,3)
				.aimAt(0,0,0);

			screen1.reset(new Mesh());
			screen1->fromFile("res/models/screen.obj")
				.moveTo(-1.05, 0, 0)
			 	.rotate(core::Transformable::X_AXIS, math::toRadian(90))
			 	.scale(vec3(viewPort.z/viewPort.w, 1, 1));

			screen2.reset(new Mesh());
			screen2->fromFile("res/models/screen.obj")
				.moveTo(1.05, 0, 0)
			 	.rotate(core::Transformable::X_AXIS, math::toRadian(90))
			 	.scale(vec3(viewPort.z/viewPort.w, 1, 1));

			screen1->material().diffuse() = vec3(1,0.5,0.5);
			screen2->material().diffuse() = vec3(0.5,1,0.5);
		 }
	}

	void Renderer::update(Game &game, sec t, sec dt)	{}

	void Renderer::render()
	{
		// FIXME temp code to check render to texture
		{
			frameBuffer->bind();
			glClearColor(0,0,0,1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
		}


		renderToActiveFrameBuffer();

		// FIXME temp code to check render to texture
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(viewPort.x, viewPort.y ,viewPort.z ,viewPort.w);
			glClearColor(0.8,0.8,0.8,1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			ShaderProgram &screenShader = getShader(0);

			screenShader.use();
			screenShader.setUniform("ambientLight", vec3(1,1,1));

		 	screenShader.setUniform("material", screen1->material());
			frameBuffer->texture(GL_COLOR_ATTACHMENT0).bind(0);
		 	screenShader.setUniform("material.texture", 0);
		 	screenShader.setUniform("M",   screen1->modelMatrix());
		 	screenShader.setUniform("MVP", fakeCamera.projectionMatrix() * fakeCamera.viewMatrix() * screen1->modelMatrix());
		 	screen1->draw();

		 	screenShader.setUniform("material", screen2->material());
			frameBuffer->texture(GL_COLOR_ATTACHMENT0).bind(0);
		 	screenShader.setUniform("material.texture", 0);
		 	screenShader.setUniform("M",   screen2->modelMatrix());
		 	screenShader.setUniform("MVP", fakeCamera.projectionMatrix() * fakeCamera.viewMatrix() * screen2->modelMatrix());
		 	screen2->draw();
		}
	 		

	}

	void Renderer::renderToActiveFrameBuffer()
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
