#include <tiny/rendering/gl/mesh.h>
#include <tiny/rendering/gl/material.h>
#include <tiny/rendering/gl/shaderprogram.h>
#include <tiny/rendering/gl/shader.h>
#include <tiny/rendering/gl/phong.h>

#include <tiny/core/game.h>
#include <tiny/core/fpscounter.h>
#include <tiny/core/transformable.h>
#include <tiny/core/camera.h>

#include <iostream>

using namespace tiny::rendering::gl;
using namespace tiny::core;
using namespace tiny::math;

class AmbientShader : public ShaderProgram
{
public:
	using ShaderProgram::setUniform;
	using ShaderProgram::use;

	AmbientShader() : ShaderProgram(ShaderProgram::fromFiles("res/shaders/ambient.vs", "res/shaders/ambient.fs")) {}

	void setUniform(const std::string &uniform, const Material &material)
	{
		setUniform(uniform + ".texture",   material.texture().bind(0));
		setUniform(uniform + ".ambient",   material.ambient());
		setUniform(uniform + ".diffuse",   material.diffuse());
		setUniform(uniform + ".specular",  material.specular());
		setUniform(uniform + ".shininess", material.shininess());
	}

	void draw(const Camera &camera, const Mesh &mesh)
	{
		use();
		setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
		setUniform("material", mesh.material());

		mesh.draw();
	}
};

class DirectionalLightShader : public ShaderProgram
{
public:	
	using ShaderProgram::setUniform;
	using ShaderProgram::use;

	DirectionalLightShader() : ShaderProgram(ShaderProgram::fromFiles("res/shaders/directional.vs", "res/shaders/directional.fs")) {}

	void setUniform(const std::string &uniform, const BaseLight &lightSource)
	{
		setUniform(uniform + ".color",     lightSource.color);
		setUniform(uniform + ".intensity", lightSource.intensity);
	}

	void setUniform(const std::string &uniform, const DirectionalLight &directionalLight)
	{
		setUniform(uniform + ".base",     dynamic_cast<const BaseLight&>(directionalLight));
		setUniform(uniform + ".direction", directionalLight.direction);
	}

	void setUniform(const std::string &uniform, const Material &material)
	{
		setUniform(uniform + ".texture",   material.texture().bind(0));
		setUniform(uniform + ".ambient",   material.ambient());
		setUniform(uniform + ".diffuse",   material.diffuse());
		setUniform(uniform + ".specular",  material.specular());
		setUniform(uniform + ".shininess", material.shininess());
	}

	void draw(const Camera &camera, const Mesh &mesh, const DirectionalLight &directionalLight)
	{
		use();
		setUniform("M",   mesh.modelMatrix());
		setUniform("MVP", camera.projectionMatrix() * camera.viewMatrix() * mesh.modelMatrix());
		setUniform("material", mesh.material());
		setUniform("directionalLight", directionalLight);
		setUniform("eyePos", camera.position());

		mesh.draw();
	}

};


class GLContext
{
public:	

	void enableBlending()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(false);
		glDepthFunc(GL_EQUAL);
	}

	void disableBlending()
	{
		glDepthFunc(GL_LESS);
		glDepthMask(true);
		glDisable(GL_BLEND);
	}

	void enableBackfaceCulling()
	{
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
	}

	void disableBackfaceCulling()
	{
		glDisable(GL_CULL_FACE);
	}

	void enableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}

	void disableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
	}
};


class Renderer
{
public:	

	void init()
	{
		mContext.enableBackfaceCulling();
		mContext.enableDepthTest();
	}

	void addDirectionalLight(DirectionalLight &&directionalLight)
	{
		mDirectionalLights.push_back(std::move(directionalLight));
	}

	void render(Camera &camera, Mesh &mesh)
	{
		mAmbientShader.draw(camera, mesh);
		mContext.enableBlending();
		for (const auto &directionalLight : mDirectionalLights)
			mDirectionalShader.draw(camera, mesh, directionalLight);
		mContext.disableBlending();
	}

private:
	AmbientShader mAmbientShader;
	DirectionalLightShader mDirectionalShader;
	GLContext mContext;

	std::vector<DirectionalLight> mDirectionalLights;
};






class MyGame : public Game
{
public:

	MyGame(Window &&window) : Game(std::move(window)) {	}

	void init()
	{

		mMeshes.push_back(Mesh::fromFiles("res/models/ground.obj", "res/models/ground.mtl"));
		mMeshes.back().moveTo(0,-2,0);

		mMeshes.push_back(Mesh::fromFiles("res/models/box.obj", "res/models/box.mtl"));
		mMeshes.back().moveTo(0,4,0);

		mMeshes.push_back(Mesh::fromFiles("res/models/sphere_hd_smooth.obj", "res/models/sphere_smooth.mtl"));

		mCamera = Camera::withPerspective(toRadian(70), window().aspect(), 0.01f, 1000.0f);
		mCamera.moveTo(0,0,5);
		mCamera.rotateTo(Transformable::Y_AXIS, toRadian(180));

		mRenderer.init();
		mRenderer.addDirectionalLight(DirectionalLight(vec3(1,1,1), 2.0f, vec3(1,-1,1)));
		mRenderer.addDirectionalLight(DirectionalLight(vec3(0,1,0), 1.0f, vec3(-1,-1,-1)));

 		window().vsync(false);				
	}


	void update(sec t, sec dt)
	{
		if (shouldStop())
			stop();

		mFPSCounter.update(dt);
		mCamera.update(window(), inputs(), dt);
	}

	void render()
	{
		for (auto &mesh: mMeshes)
			mRenderer.render(mCamera, mesh);

		mFPSCounter.newFrame();
	}

	bool shouldStop()
	{
		if (inputs().isWindowCloseRequested())
			return true;

		if (inputs().isKeyHeld(Key::KEY_LEFT_CMD) && inputs().isKeyPressed(Key::KEY_Z))
			return true;

		return false;
	}

private:	
	FPSCounter mFPSCounter;
	Renderer mRenderer;
	Camera mCamera; 

	std::vector<Mesh> mMeshes;
};


int main(int argc, char **argv)
{
	MyGame(Window(1900, 1200, "MyGame")).start();
	return EXIT_SUCCESS;
}