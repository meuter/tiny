#include <tiny/rendering/gl/mesh.h>
#include <tiny/rendering/gl/material.h>
#include <tiny/rendering/gl/renderer.h>
#include <tiny/rendering/gl/context.h>
#include <tiny/rendering/gl/scene.h>

#include <tiny/core/game.h>
#include <tiny/core/fpscounter.h>
#include <tiny/core/transformable.h>
#include <tiny/core/camera.h>
#include <tiny/core/controls.h>
#include <tiny/core/lightsource.h>

using namespace tiny::rendering::gl;
using namespace tiny::rendering;
using namespace tiny::core;
using namespace tiny::math;

int main(int argc, char **argv)
{
	Game mGame(Window(1900, 1200, "MyGame", Window::OPENGL));	
	Context mContext(mGame.window());
	Scene mScene;

	mScene.addMesh("ground")
		.fromFiles("res/models/ground.obj", "res/models/ground.mtl")
		.moveTo(0,-2,0);

	mScene.addMesh("box")
		.fromFiles("res/models/box.obj", "res/models/box.mtl")
		.attachTo(mScene.getMesh("ground"))
		.moveTo(0,6,0);

	mScene.addMesh("sphere")
		.fromFiles("res/models/sphere_hd_smooth.obj", "res/models/sphere_smooth.mtl")
		.attachTo(mScene.getMesh("ground"))
		.moveTo(0,2,0);

	mScene.setAmbient(vec3(1,1,1) * 0.2f);	
	mScene.camera()
		.withPerspective(toRadian(70), mGame.window().aspect(), 0.01f, 1000.0f)
		.moveTo(0,0,7)
		.aimAt(0,0,0);

	mScene.addLight("d1").directional(vec3(1,1,1) * 0.7f, vec3(1,-1,1));
	mScene.addLight("d2").directional(vec3(0,1,0) * 0.5f, vec3(-1,-1,-1));

	mScene.addLight("p1").point(vec3(1,0,0) * 0.4f, vec3(1,-1.5f,1));
	mScene.addLight("p2").point(vec3(0,1,0) * 0.6f, vec3(2,-1.5f,2));
	mScene.addLight("p3").point(vec3(0,0,1) * 0.6f, vec3(3,-1.5f,3));

	mScene.addLight("s1").spot(vec3(1,1,0) * 1.0f, vec3(-2,-1.9,2), vec3(1,0,-1), 0.6f, 10);
	mScene.addLight("s2").spot(vec3(0,1,1) * 1.0f, vec3(-3,-1.9,3), vec3(1,0,-1), 0.6f, 10);

 	mContext.vsync(false);			
 
 	mGame.add(FPSCounter());
 	mGame.add(WindowControl());
 	mGame.add(KeyboardControl(mScene.camera()));
 	mGame.add(MouseControl(mScene.camera()));
 	mGame.add(Renderer(mContext, mScene));

 	mGame.start();
 	return EXIT_SUCCESS;
}
