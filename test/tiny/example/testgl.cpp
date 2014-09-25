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

#include <iostream>

using namespace tiny::rendering::gl;
using namespace tiny::rendering;
using namespace tiny::core;
using namespace tiny::math;

class KeyboardControl : public Game::Component
{
public:
	KeyboardControl(Transformable &controlled) : mControlled(controlled) {}
	virtual ~KeyboardControl() = default;

	void update(Game &game, sec t, sec dt)
	{
		float amount = dt.count() * 10;

		if (game.inputs().isKeyHeld(Key::KEY_UP))
			mControlled.move(mControlled.forward(), amount);
		if (game.inputs().isKeyHeld(Key::KEY_DOWN))
			mControlled.move(mControlled.backward(), amount);
		if (game.inputs().isKeyHeld(Key::KEY_LEFT))
			mControlled.move(mControlled.left(), amount);
		if (game.inputs().isKeyHeld(Key::KEY_RIGHT))
			mControlled.move(mControlled.right(), amount);
		if (game.inputs().isKeyHeld(Key::KEY_PAGEUP))
			mControlled.move(mControlled.up(), amount);
		if (game.inputs().isKeyHeld(Key::KEY_PAGEDOWN))
			mControlled.move(mControlled.down(), amount);	
	}

private:
	Transformable &mControlled;
};

class MouseControl : public Game::Component
{
public:
	MouseControl(Transformable &controlled) : mControlled(controlled) {}
	virtual ~MouseControl() = default;

	void update(Game &game, sec t, sec dt)
	{
		const float sensitivity = 0.005f;

		if (game.inputs().isMouseReleased(MouseButton::MIDDLE))
		{
			game.inputs().showMouseCursor(true);		
		}
		else if (game.inputs().isMousePressed(MouseButton::MIDDLE))
		{
			game.inputs().showMouseCursor(false);
			game.inputs().setMousePosition(game.window().center());
		}
		else if (game.inputs().isMouseHeld(MouseButton::MIDDLE))
		{
	 		auto dpos = game.window().center() - game.inputs().getMousePosition();

			if (dpos.x != 0)
				mControlled.rotate(mControlled.up(), rad{dpos.x * sensitivity});

			if (dpos.y != 0)
				mControlled.rotate(mControlled.right(), rad{dpos.y * sensitivity});

			if (dpos.x != 0 || dpos.y != 0)
				game.inputs().setMousePosition(game.window().center());
		}
	}

	void render() {}

private:
	Transformable &mControlled;
};

class WindowControl : public Game::Component
{
public:	
	WindowControl()  {}
	virtual ~WindowControl() = default;

	void update(Game &game, sec t, sec dt)
	{
		if (game.inputs().isWindowCloseRequested())
			game.stop();

		if (game.inputs().isKeyHeld(Key::KEY_LEFT_CMD) && game.inputs().isKeyPressed(Key::KEY_Z))
			game.stop();
	}
};




class MyGame : public Game
{
public:

	MyGame(Window &&window) 
		: Game(std::move(window)),
		  mContext(this->window()), 
		  mRenderer(mContext)    {}

	void init()
	{
		mScene.addMesh("ground", Mesh::fromFiles("res/models/ground.obj", "res/models/ground.mtl")).moveTo(0,-2,0);
		mScene.addMesh("box",    Mesh::fromFiles("res/models/box.obj", "res/models/box.mtl")).attachTo(mScene.getMesh("ground")).moveTo(0,6,0);
		mScene.addMesh("sphere", Mesh::fromFiles("res/models/sphere_hd_smooth.obj", "res/models/sphere_smooth.mtl")).attachTo(mScene.getMesh("ground")).moveTo(0,2,0);

		mScene.setAmbient(vec3(1,1,1) * 0.2f);
		mScene.setCamera(Camera::withPerspective(toRadian(70), window().aspect(), 0.01f, 1000.0f)).moveTo(0,0,7).aimAt(0,0,0);

		mScene.addLight("d1", LightSource::directional(vec3(1,1,1) * 0.7f, vec3(1,-1,1)));
		mScene.addLight("d2", LightSource::directional(vec3(0,1,0) * 0.5f, vec3(-1,-1,-1)));

		mScene.addLight("p1", LightSource::point(vec3(1,0,0) * 0.4f, vec3(1,-1.5f,1)));
		mScene.addLight("p2", LightSource::point(vec3(0,1,0) * 0.6f, vec3(2,-1.5f,2)));
		mScene.addLight("p3", LightSource::point(vec3(0,0,1) * 0.6f, vec3(3,-1.5f,3)));

		mScene.addLight("s1", LightSource::spot(vec3(1,1,0) * 1.0f, vec3(-2,-1.9,2), vec3(1,0,-1), 0.6f, 10));
		mScene.addLight("s2", LightSource::spot(vec3(0,1,1) * 1.0f, vec3(-3,-1.9,3), vec3(1,0,-1), 0.6f, 10));

		mRenderer.init();

		mCameraKeyboardControl.reset(new KeyboardControl(mScene.camera()));
		mCameraMouseControl.reset(new MouseControl(mScene.camera()));

 	 	mContext.vsync(false);			
	}


	void update(sec t, sec dt)
	{
		mFPSCounter.update(*this, t, dt);
		mWindowControl.update(*this, t, dt);
		mCameraKeyboardControl->update(*this, t, dt);
		mCameraMouseControl->update(*this, t, dt);
	}

	void render()
	{
		mRenderer.render(mScene);
		mFPSCounter.render();
	}


private:	
	Context mContext;
	Renderer mRenderer;
	Scene mScene;

	// std::vector<std::unique_ptr<Game::Component>> mComponents;

	FPSCounter mFPSCounter;
	std::unique_ptr<KeyboardControl> mCameraKeyboardControl;
	std::unique_ptr<MouseControl> mCameraMouseControl;
	WindowControl mWindowControl;
};


int main(int argc, char **argv)
{
	MyGame(Window(1900, 1200, "MyGame", Window::OPENGL)).start();
	return EXIT_SUCCESS;
}