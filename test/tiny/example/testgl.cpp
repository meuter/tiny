#include <tiny/rendering/gl/mesh.h>
#include <tiny/rendering/gl/material.h>
#include <tiny/rendering/gl/renderer.h>
#include <tiny/rendering/gl/context.h>
#include <tiny/rendering/gl/scene.h>
#include <tiny/rendering/lightsource.h>

#include <tiny/core/game.h>
#include <tiny/core/fpscounter.h>
#include <tiny/core/transformable.h>
#include <tiny/core/camera.h>
#include <tiny/core/controls.h>

#include <iostream>

using namespace tiny::rendering::gl;
using namespace tiny::rendering;
using namespace tiny::core;
using namespace tiny::math;

class MyGame : public Game
{
public:
	MyGame(Window &&window)	: Game(std::move(window)),  mContext(this->window())
	{
		mScene.addMesh("ground", Mesh::fromFiles("res/models/ground.obj", "res/models/ground.mtl")).moveTo(0,-2,0);
		mScene.addMesh("box",    Mesh::fromFiles("res/models/box.obj", "res/models/box.mtl")).attachTo(mScene.getMesh("ground")).moveTo(0,6,0);
		mScene.addMesh("sphere", Mesh::fromFiles("res/models/sphere_hd_smooth.obj", "res/models/sphere_smooth.mtl")).attachTo(mScene.getMesh("ground")).moveTo(0,2,0);

		mScene.setAmbient(vec3(1,1,1) * 0.2f);
		mScene.setCamera(Camera::withPerspective(toRadian(70), this->window().aspect(), 0.01f, 1000.0f)).moveTo(0,0,7).aimAt(0,0,0);

		mScene.addLight("d1", LightSource::directional(vec3(1,1,1) * 0.7f, vec3(1,-1,1)));
		mScene.addLight("d2", LightSource::directional(vec3(0,1,0) * 0.5f, vec3(-1,-1,-1)));

		mScene.addLight("p1", LightSource::point(vec3(1,0,0) * 0.4f, vec3(1,-1.5f,1)));
		mScene.addLight("p2", LightSource::point(vec3(0,1,0) * 0.6f, vec3(2,-1.5f,2)));
		mScene.addLight("p3", LightSource::point(vec3(0,0,1) * 0.6f, vec3(3,-1.5f,3)));

		mScene.addLight("s1", LightSource::spot(vec3(1,1,0) * 1.0f, vec3(-2,-1.9,2), vec3(1,0,-1), 0.6f, 10));
		mScene.addLight("s2", LightSource::spot(vec3(0,1,1) * 1.0f, vec3(-3,-1.9,3), vec3(1,0,-1), 0.6f, 10));

 	 	mContext.vsync(false);			

 	 	addComponent(new FPSCounter());
 	 	addComponent(new WindowControl());
 	 	addComponent(new KeyboardControl(mScene.camera()));
 	 	addComponent(new MouseControl(mScene.camera()));
 	 	addComponent(new Renderer(mContext, mScene));
	}

private:	
	Context mContext;
	Scene mScene;

};


int main(int argc, char **argv)
{
	MyGame(Window(1900, 1200, "MyGame", Window::OPENGL)).start();
	return EXIT_SUCCESS;
}