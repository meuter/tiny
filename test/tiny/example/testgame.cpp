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

		for (int i = 0; i < 26; ++i)
		{
			Mesh face = Mesh::fromFile("res/models/rubics.obj", i);
			face.setMaterial(Material::fromFile("res/models/rubics.mtl"));
			face.moveTo(0,2,0);
			mCube.push_back(std::move(face));
		}

		mCamera = Camera::withPerspective(toRadian(70), window().aspect(), 0.01f, 1000.0f);
		mCamera.moveTo(0,0,5);
		mCamera.rotateTo(Transformable::Y_AXIS, toRadian(180));

		mRenderer.init();
		mRenderer.addDirectionalLight(DirectionalLight(vec3(1,1,1), 1.0f, vec3(-1,-1,1)));
		mRenderer.addDirectionalLight(DirectionalLight(vec3(1,1,1), 1.0f, vec3(1,-1,-1)));
		// mRenderer.addDirectionalLight(DirectionalLight(vec3(1,1,1), 1.5f, vec3(1,0,-1)));
		// mRenderer.addDirectionalLight(DirectionalLight(vec3(1,1,1), 1.5f, vec3(-1,0,1)));


		// mRenderer.addPointLight(PointLight(vec3(1,0,0), 0.4f, vec3(1,-1.5f,1)));
		// mRenderer.addPointLight(PointLight(vec3(0,1,0), 0.6f, vec3(2,-1.5f,2)));
		// mRenderer.addPointLight(PointLight(vec3(0,0,1), 0.8f, vec3(3,-1.5f,3)));
 
		// mRenderer.addSpotLight(SpotLight(vec3(1,1,0), 1.0f, vec3(-2,-1.9,2), vec3(1,0,-1)));

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
		for (const auto &mesh: mCube)
			mRenderer.render(mCamera, mesh);
		mRenderer.render(mCamera, mGround);
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
	std::vector<Mesh> mCube;
	Mesh mGround;
};


int main(int argc, char **argv)
{
	MyGame(Window(1900, 1200, "MyGame")).start();
	return EXIT_SUCCESS;
}