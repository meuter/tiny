#include <tiny/rendering/gl/mesh.h>
#include <tiny/rendering/gl/material.h>
#include <tiny/rendering/gl/renderer.h>
#include <tiny/rendering/gl/context.h>

#include <tiny/core/game.h>
#include <tiny/core/fpscounter.h>
#include <tiny/core/transformable.h>
#include <tiny/core/camera.h>

#include <iostream>

using namespace tiny::rendering::gl;
using namespace tiny::core;
using namespace tiny::math;


class MyGame : public Game
{
public:

	MyGame(Window &&window) : Game(std::move(window)), mContext(this->window()), mRenderer(mContext) {}

	void init()
	{
		mGround = Mesh::fromFiles("res/models/ground.obj", "res/models/ground.mtl");
		mGround.moveTo(0,-2,0);

		mBox = Mesh::fromFiles("res/models/box.obj", "res/models/box.mtl");
		mBox.moveTo(0,4,0);

		mSphere = Mesh::fromFiles("res/models/sphere_hd_smooth.obj", "res/models/sphere_smooth.mtl");

		mCamera = Camera::withPerspective(toRadian(70), window().aspect(), 0.01f, 1000.0f);
		mCamera.moveTo(0,0,7);
		mCamera.rotateTo(Transformable::Y_AXIS, toRadian(180));

		mRenderer.init();

		mRenderer.setAmbientLight(vec3(1,1,1), 0.2f);

		mRenderer.addDirectionalLight(vec3(1,1,1), 0.7f, vec3(1,-1,1));
		mRenderer.addDirectionalLight(vec3(0,1,0), 0.5f, vec3(-1,-1,-1));

		mRenderer.addPointLight(vec3(1,0,0), 0.4f, vec3(1,-1.5f,1));
		mRenderer.addPointLight(vec3(0,1,0), 0.6f, vec3(2,-1.5f,2));
		mRenderer.addPointLight(vec3(0,0,1), 0.6f, vec3(3,-1.5f,3));

		mRenderer.addSpotLight(vec3(1,1,0), 1.0f, vec3(-2,-1.9,2), vec3(1,0,-1), 0.6f, 10);
		mRenderer.addSpotLight(vec3(0,1,1), 1.0f, vec3(-3,-1.9,3), vec3(1,0,-1), 0.6f, 10);

 	 	mContext.vsync(false);			
	}


	void update(sec t, sec dt)
	{
		if (shouldStop())
			stop();

		mFPSCounter.update(t, dt);
		mCamera.update(window(), inputs(), dt);
	}

	void render()
	{
		mRenderer.render(mCamera, mGround);
		mRenderer.render(mCamera, mBox);
		mRenderer.render(mCamera, mSphere);

		mFPSCounter.render();
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
	Context mContext;
	Renderer mRenderer;
	FPSCounter mFPSCounter;
	Camera mCamera;
	Mesh mGround, mSphere, mBox;
};


int main(int argc, char **argv)
{
	MyGame(Window(1900, 1200, "MyGame", Window::OPENGL)).start();
	return EXIT_SUCCESS;
}