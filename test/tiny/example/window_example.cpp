#include <tiny/rendering/shaderprogram.h>
#include <tiny/rendering/shader.h>
#include <tiny/rendering/mesh.h>
#include <tiny/rendering/material.h>
#include <tiny/rendering/gl/phong.h>

#include <tiny/core/game.h>
#include <tiny/core/fpscounter.h>
#include <tiny/core/transformable.h>
#include <tiny/core/camera.h>

#include <iostream>

using namespace tiny::rendering;
using namespace tiny::core;
using namespace tiny::math;

class MyGame : public Game
{
public:

	MyGame(Window &&window) : Game(std::move(window)) {	}

	void init()
	{
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		mMeshes.push_back(Mesh::fromFiles("res/models/ground.obj", "res/models/ground.mtl"));
		mMeshes.back().moveTo(0,-2,0);

		mMeshes.push_back(Mesh::fromFiles("res/models/box.obj", "res/models/box.mtl"));
		mMeshes.back().moveTo(0,4,0);

		mMeshes.push_back(Mesh::fromFiles("res/models/sphere_smooth.obj", "res/models/sphere_smooth.mtl"));

		mCamera = Camera::withPerspective(toRadian(70), window().aspect(), 0.01f, 1000.0f);

		mShaderProgram.setDirectionalLight(gl::DirectionalLight(vec3(1,1,1)*0.7f, 1.0f, vec3(0,-1,0)));

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
			mShaderProgram.draw(mCamera, mesh);
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
	gl::PhongShader mShaderProgram;
	Camera mCamera; 
	FPSCounter mFPSCounter;
	std::vector<Mesh> mMeshes;
};


int main(int argc, char **argv)
{
	MyGame(Window(1900, 1200, "MyGame")).start();
	return EXIT_SUCCESS;
}