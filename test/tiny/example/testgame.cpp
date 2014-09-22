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
		mCamera.moveTo(0,0,5);
		mCamera.rotateTo(Transformable::Y_AXIS, toRadian(180));

		mRenderer.init();
		mRenderer.addDirectionalLight(DirectionalLight(vec3(1,1,1), 1.0f, vec3(1,-1,1)));
		mRenderer.addDirectionalLight(DirectionalLight(vec3(0,1,0), 0.5f, vec3(-1,-1,-1)));

		mRenderer.addPointLight(PointLight(vec3(1,0,0), 0.4f, vec3(1,-1.0f,1)));
		mRenderer.addPointLight(PointLight(vec3(0,1,0), 0.6f, vec3(2,-1.0f,2)));
		mRenderer.addPointLight(PointLight(vec3(0,0,1), 0.8f, vec3(3,-1.0f,3)));
 
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
		mRenderer.render(mCamera, mSphere);
		mRenderer.render(mCamera, mBox);
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
	Mesh mBox, mSphere, mGround;
	Renderer mRenderer;
	FPSCounter mFPSCounter;
	Camera mCamera;
};


int main(int argc, char **argv)
{
	MyGame(Window(1900, 1200, "MyGame")).start();
	return EXIT_SUCCESS;
}