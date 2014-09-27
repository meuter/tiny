#include "renderer.h"
#include "framebuffer.h"


namespace tiny { namespace rendering { namespace gl {

	// FIXME temp code to check render to texture
	core::Camera fakeCamera;
	std::unique_ptr<Mesh> colorScreen, texCoordScreen, normalScreen, positionScreen;
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

			frameBuffer.reset(new FrameBuffer(viewPort.z, viewPort.w, { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 } ) );

			fakeCamera.withPerspective(math::toRadian(70), viewPort.z/viewPort.w, 0.01f, 1000.0f)
				.moveTo(0,0,3)
				.aimAt(0,0,0);

			colorScreen.reset(new Mesh());
			colorScreen->fromFile("res/models/screen.obj")
				.moveTo(-1.05, -1.05, 0)
			 	.rotate(core::Transformable::X_AXIS, math::toRadian(90))
			 	.scale(vec3(viewPort.z/viewPort.w, 1, 1));

			texCoordScreen.reset(new Mesh());
			texCoordScreen->fromFile("res/models/screen.obj")
				.moveTo(1.05, 1.05, 0)
			 	.rotate(core::Transformable::X_AXIS, math::toRadian(90))
			 	.scale(vec3(viewPort.z/viewPort.w, 1, 1));

			normalScreen.reset(new Mesh());
			normalScreen->fromFile("res/models/screen.obj")
				.moveTo(-1.05, 1.05, 0)
			 	.rotate(core::Transformable::X_AXIS, math::toRadian(90))
			 	.scale(vec3(viewPort.z/viewPort.w, 1, 1));

			positionScreen.reset(new Mesh());
			positionScreen->fromFile("res/models/screen.obj")
				.moveTo(1.05, -1.05, 0)
			 	.rotate(core::Transformable::X_AXIS, math::toRadian(90))
			 	.scale(vec3(viewPort.z/viewPort.w, 1, 1));



			colorScreen->material().diffuse() = vec3(1,1,1);
			texCoordScreen->material().diffuse() = vec3(1,1,1);
			normalScreen->material().diffuse() = vec3(1,1,1);
			positionScreen->material().diffuse() = vec3(1,1,1);
		 }
	}

	void Renderer::update(Game &game, sec t, sec dt)	{}

	void Renderer::render()
	{
		// renderToActiveFrameBuffer();
		renderOnFourScreens();
	}

	void Renderer::renderOnFourScreens()
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

		 	screenShader.setUniform("material", colorScreen->material());
			frameBuffer->texture(GL_COLOR_ATTACHMENT0).bind(0);
		 	screenShader.setUniform("material.texture", 0);
		 	screenShader.setUniform("M",   colorScreen->modelMatrix());
		 	screenShader.setUniform("MVP", fakeCamera.projectionMatrix() * fakeCamera.viewMatrix() * colorScreen->modelMatrix());
		 	colorScreen->draw();

		 	screenShader.setUniform("material", texCoordScreen->material());
			frameBuffer->texture(GL_COLOR_ATTACHMENT1).bind(0);
		 	screenShader.setUniform("material.texture", 0);
		 	screenShader.setUniform("M",   texCoordScreen->modelMatrix());
		 	screenShader.setUniform("MVP", fakeCamera.projectionMatrix() * fakeCamera.viewMatrix() * texCoordScreen->modelMatrix());
		 	texCoordScreen->draw();

		 	screenShader.setUniform("material", normalScreen->material());
			frameBuffer->texture(GL_COLOR_ATTACHMENT2).bind(0);
		 	screenShader.setUniform("material.texture", 0);
		 	screenShader.setUniform("M",   normalScreen->modelMatrix());
		 	screenShader.setUniform("MVP", fakeCamera.projectionMatrix() * fakeCamera.viewMatrix() * normalScreen->modelMatrix());
		 	normalScreen->draw();

		 	screenShader.setUniform("material", positionScreen->material());
			frameBuffer->texture(GL_COLOR_ATTACHMENT3).bind(0);
		 	screenShader.setUniform("material.texture", 0);
		 	screenShader.setUniform("M",   positionScreen->modelMatrix());
		 	screenShader.setUniform("MVP", fakeCamera.projectionMatrix() * fakeCamera.viewMatrix() * positionScreen->modelMatrix());
		 	positionScreen->draw();
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
