#include <tiny/rendering/mesh.h>
#include <tiny/rendering/gl/material.h>
#include <tiny/rendering/gl/shaderprogram.h>
#include <tiny/rendering/gl/shader.h>
#include <tiny/rendering/gl/phong.h>

#include <tiny/core/game.h>
#include <tiny/core/fpscounter.h>
#include <tiny/core/transformable.h>
#include <tiny/core/camera.h>

#include <iostream>

using namespace tiny::rendering;
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

	void draw(Camera &camera, Mesh &mesh)
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

	void setUniform(const std::string &uniform, const gl::BaseLight &lightSource)
	{
		setUniform(uniform + ".color",     lightSource.color);
		setUniform(uniform + ".intensity", lightSource.intensity);
	}

	void setUniform(const std::string &uniform, const gl::DirectionalLight &directionalLight)
	{
		setUniform(uniform + ".base",     dynamic_cast<const gl::BaseLight&>(directionalLight));
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

	void draw(Camera &camera, Mesh &mesh, gl::DirectionalLight &directionalLight)
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

class MyGame : public Game
{
public:

	MyGame(Window &&window) : Game(std::move(window)) {	}

	void init()
	{
		mContext.enableBackfaceCulling();
		mContext.enableDepthTest();

		glEnable(GL_DEPTH_TEST);

		mMeshes.push_back(Mesh::fromFiles("res/models/ground.obj", "res/models/ground.mtl"));
		mMeshes.back().moveTo(0,-2,0);

		mMeshes.push_back(Mesh::fromFiles("res/models/box.obj", "res/models/box.mtl"));
		mMeshes.back().moveTo(0,4,0);

		mMeshes.push_back(Mesh::fromFiles("res/models/sphere_hd_smooth.obj", "res/models/sphere_smooth.mtl"));

		mCamera = Camera::withPerspective(toRadian(70), window().aspect(), 0.01f, 1000.0f);

		mWhiteSun = gl::DirectionalLight(vec3(1,1,1), 2.0f, vec3(1,-1,1));
		mBlueMoon = gl::DirectionalLight(vec3(0,1,0), 1.0f, vec3(-1,-1,-1));

		mCamera.moveTo(0,0,5);
		mCamera.rotateTo(Transformable::Y_AXIS, toRadian(180));

 		window().vsync(false);				
	}


	void update(sec t, sec dt)
	{
		if (shouldStop())
			stop();

		mFPSCounter.update(dt);

		moveCamera(dt);
		lookCamera(dt);
	}

	void render()
	{
		for (auto &mesh: mMeshes)
		{
			mAmbientShader.draw(mCamera, mesh);
			mContext.enableBlending();
			mDirectionalShader.draw(mCamera, mesh, mWhiteSun);
			mDirectionalShader.draw(mCamera, mesh, mBlueMoon);
			mContext.disableBlending();
		}
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

	void moveCamera(sec dt)
	{
		float amount = dt.count() * 10;

		if (inputs().isKeyHeld(Key::KEY_UP))
			mCamera.move(mCamera.forward(), amount);
		if (inputs().isKeyHeld(Key::KEY_DOWN))
			mCamera.move(mCamera.backward(), amount);
		if (inputs().isKeyHeld(Key::KEY_LEFT))
			mCamera.move(mCamera.left(), amount);
		if (inputs().isKeyHeld(Key::KEY_RIGHT))
			mCamera.move(mCamera.right(), amount);
		if (inputs().isKeyHeld(Key::KEY_PAGEUP))
			mCamera.move(mCamera.up(), amount);
		if (inputs().isKeyHeld(Key::KEY_PAGEDOWN))
			mCamera.move(mCamera.down(), amount);
	}

	void lookCamera(sec dt)
	{
		const float sensitivity = 0.005f;

		if (inputs().isMouseReleased(MouseButton::MIDDLE))
		{
			inputs().showMouseCursor(true);		
		}
		else if (inputs().isMousePressed(MouseButton::MIDDLE))
		{
			inputs().showMouseCursor(false);
			inputs().setMousePosition(window().center());
		}
		else if (inputs().isMouseHeld(MouseButton::MIDDLE))
		{
	 		auto dpos = window().center() - inputs().getMousePosition();

			if (dpos.x != 0)
				mCamera.rotate(mCamera.up(), rad{dpos.x * sensitivity});

			if (dpos.y != 0)
				mCamera.rotate(mCamera.right(), rad{dpos.y * sensitivity});

			if (dpos.x != 0 || dpos.y != 0)
				inputs().setMousePosition(window().center());
		}
	}

private:	
	AmbientShader mAmbientShader;
	DirectionalLightShader mDirectionalShader;
	Camera mCamera; 
	FPSCounter mFPSCounter;
	std::vector<Mesh> mMeshes;
	gl::DirectionalLight mWhiteSun, mBlueMoon;
	GLContext mContext;
};


int main(int argc, char **argv)
{
	MyGame(Window(1900, 1200, "MyGame")).start();
	return EXIT_SUCCESS;
}