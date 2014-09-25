#include <tiny/rendering/gl/mesh.h>
#include <tiny/rendering/gl/material.h>
#include <tiny/rendering/gl/renderer.h>
#include <tiny/rendering/gl/context.h>
#include <tiny/rendering/lightsource.h>

#include <tiny/core/game.h>
#include <tiny/core/fpscounter.h>
#include <tiny/core/transformable.h>
#include <tiny/core/camera.h>

#include <iostream>

using namespace tiny::rendering::gl;
using namespace tiny::rendering;
using namespace tiny::core;
using namespace tiny::math;


class Scene
{
public:

	LightSource &addLight(const std::string &name, const LightSource &light)
	{
		return (mLightSources[name] = light);
	}

	Mesh &addMesh(const std::string &name, Mesh &&mesh)
	{
		return (mMeshes[name] = std::move(mesh));
	}

	Mesh &getMesh(const std::string &name)
	{
		auto hit = mMeshes.find(name);

		if (hit == mMeshes.end())
			throw std::runtime_error("mesh not found");

		return hit->second;
	}

	Camera &setCamera(const Camera &camera)
	{
		mCamera = camera;
		return mCamera;
	}

	vec3 &setAmbient(const vec3 &color)
	{
		mAmbient = color;
		return mAmbient;
	}

	std::map<std::string, LightSource> mLightSources;
	std::map<std::string, Mesh> mMeshes;
	Camera mCamera;
	vec3 mAmbient;
};

class MyGame : public Game
{
public:

	MyGame(Window &&window) : Game(std::move(window)), mContext(this->window()), mRenderer(mContext) {}

	void init()
	{

		mScene.addMesh("ground", Mesh::fromFiles("res/models/ground.obj", "res/models/ground.mtl"));
		mScene.addMesh("box",    Mesh::fromFiles("res/models/box.obj", "res/models/box.mtl"));
		mScene.addMesh("sphere", Mesh::fromFiles("res/models/sphere_hd_smooth.obj", "res/models/sphere_smooth.mtl"));

		mScene.setAmbient(vec3(1,1,1) * 0.2f);
		mScene.setCamera(Camera::withPerspective(toRadian(70), window().aspect(), 0.01f, 1000.0f)).moveTo(0,0,7).aimAt(0,0,0);

		mScene.addLight("d1", LightSource::directional(vec3(1,1,1) * 0.7f, vec3(1,-1,1)));
		mScene.addLight("d2", LightSource::directional(vec3(0,1,0) * 0.5f, vec3(-1,-1,-1)));

		mScene.addLight("p1", LightSource::point(vec3(1,0,0) * 0.4f, vec3(1,-1.5f,1)));
		mScene.addLight("p2", LightSource::point(vec3(0,1,0) * 0.6f, vec3(2,-1.5f,2)));
		mScene.addLight("p3", LightSource::point(vec3(0,0,1) * 0.6f, vec3(3,-1.5f,3)));

		mScene.addLight("s1", LightSource::spot(vec3(1,1,0) * 1.0f, vec3(-2,-1.9,2), vec3(1,0,-1), 0.6f, 10));
		mScene.addLight("s2", LightSource::spot(vec3(0,1,1) * 1.0f, vec3(-3,-1.9,3), vec3(1,0,-1), 0.6f, 10));

		mScene.getMesh("sphere").attachTo(mScene.getMesh("ground")).moveTo(0,2,0);
		mScene.getMesh("box").attachTo(mScene.getMesh("ground")).moveTo(0,6,0);
		mScene.getMesh("ground").moveTo(0,-2,0);


		mRenderer.init();
		mRenderer.setAmbientLight(mScene.mAmbient);
		for (const auto &light: mScene.mLightSources)
			mRenderer.add(light.second);

 	 	mContext.vsync(false);			
	}


	void update(sec t, sec dt)
	{
		if (shouldStop())
			stop();

		mFPSCounter.update(t, dt);
		mScene.mCamera.update(window(), inputs(), dt);
	}

	void render()
	{
		mRenderer.render(mScene.mCamera, mScene.getMesh("ground"));
		mRenderer.render(mScene.mCamera, mScene.getMesh("box"));
		mRenderer.render(mScene.mCamera, mScene.getMesh("sphere"));

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
	Scene mScene;
};


int main(int argc, char **argv)
{
	MyGame(Window(1900, 1200, "MyGame", Window::OPENGL)).start();
	return EXIT_SUCCESS;
}