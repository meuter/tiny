#include "renderer.h"
#include "framebuffer.h"


namespace tiny { namespace rendering { namespace gl {

	std::unique_ptr<FrameBuffer> frameBuffer;
	core::ivec4 viewPort;

	Renderer::Renderer(Context &context, Scene &scene) : mContext(context), mScene(scene) {}
	
	void Renderer::init()
	{
	 	mContext.vsync(false);
		mContext.enableBackfaceCulling();
		mContext.enableDepthTest();

		mGeometryShader = ShaderProgram::fromFiles("res/shaders/geometry.vs", "res/shaders/geometry.fs");

		// FIXME TEMP CODE
		viewPort = mContext.getViewPort();
		frameBuffer.reset(new FrameBuffer(viewPort.z, viewPort.w, { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 } ) );
	}

	void Renderer::update(Game &game, sec t, sec dt) {}

	void Renderer::render()
	{
		forwardRender();
		// deferredRender();
	}


	void Renderer::geometryPass()
	{
		frameBuffer->bind(GL_DRAW_FRAMEBUFFER);
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			

		mGeometryShader.use();

		mScene.forAll([&](const Mesh &mesh)
		{
		 	mGeometryShader.setUniform("M",   mesh.modelMatrix());
		 	mGeometryShader.setUniform("MVP", mScene.camera().projectionMatrix() * mScene.camera().viewMatrix() * mesh.modelMatrix());
		 	mGeometryShader.setUniform("material", mesh.material());
		 	mesh.draw();
		});
	}

	void Renderer::dumpGBuffer()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glViewport(viewPort.x, viewPort.y ,viewPort.z ,viewPort.w);
		glClearColor(0.8,0.8,0.8,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		frameBuffer->bind(GL_READ_FRAMEBUFFER);

		GLsizei WINDOW_WIDTH = viewPort.z;
		GLsizei WINDOW_HEIGHT = viewPort.w;
		GLsizei HalfWidth = viewPort.z / 2.0f;
		GLsizei HalfHeight = viewPort.w / 2.0f;

		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		glReadBuffer(GL_COLOR_ATTACHMENT1);
	    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, HalfHeight, HalfWidth, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		glReadBuffer(GL_COLOR_ATTACHMENT2);
	    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, HalfWidth, HalfHeight, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		glReadBuffer(GL_COLOR_ATTACHMENT3);
	    glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, HalfWidth, 0, WINDOW_WIDTH, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);	
	}

	void Renderer::deferredRender()
	{
		geometryPass();
		dumpGBuffer();
	}

	void Renderer::forwardRender()
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
