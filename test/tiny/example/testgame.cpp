#include <tiny/rendering/gl/mesh.h>
#include <tiny/rendering/gl/scene.h>
#include <tiny/rendering/gl/material.h>
#include <tiny/rendering/gl/shaderprogram.h>
#include <tiny/rendering/gl/shader.h>
#include <tiny/rendering/gl/phong.h>
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
		mScene.add(Mesh::fromFiles("res/models/ground.obj", "res/models/ground.mtl")).moveTo(0,-2,0);
		mScene.add(Mesh::fromFiles("res/models/box.obj", "res/models/box.mtl")).moveTo(0,4,0);
		mScene.add(Mesh::fromFiles("res/models/sphere_hd_smooth.obj", "res/models/sphere_smooth.mtl"));

		mCamera = Camera::withPerspective(toRadian(70), window().aspect(), 0.01f, 1000.0f);
		mCamera.moveTo(0,0,5);
		mCamera.rotateTo(Transformable::Y_AXIS, toRadian(180));

		mRenderer.init();
		mRenderer.addDirectionalLight(DirectionalLight(vec3(1,1,1), 2.0f, vec3(1,-1,1)));
		mRenderer.addDirectionalLight(DirectionalLight(vec3(0,1,0), 1.0f, vec3(-1,-1,-1)));

 	 	mContext.vsync(false);				
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
		mRenderer.render(mCamera, mScene);
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
	Context mContext;
	PhongRenderer mRenderer;
	FPSCounter mFPSCounter;
	Camera mCamera; 
	Scene mScene;
};


int main(int argc, char **argv)
{
	MyGame(Window(1900, 1200, "MyGame")).start();
	return EXIT_SUCCESS;
}