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

class KeyboardControl
{
public:
	KeyboardControl(Inputs &inputs, Transformable &controlled) : mInputs(inputs), mControlled(controlled) {}
	virtual ~KeyboardControl() = default;

	void init() {}

	void update(sec t, sec dt)
	{
		float amount = dt.count() * 10;

		if (mInputs.isKeyHeld(Key::KEY_UP))
			mControlled.move(mControlled.forward(), amount);
		if (mInputs.isKeyHeld(Key::KEY_DOWN))
			mControlled.move(mControlled.backward(), amount);
		if (mInputs.isKeyHeld(Key::KEY_LEFT))
			mControlled.move(mControlled.left(), amount);
		if (mInputs.isKeyHeld(Key::KEY_RIGHT))
			mControlled.move(mControlled.right(), amount);
		if (mInputs.isKeyHeld(Key::KEY_PAGEUP))
			mControlled.move(mControlled.up(), amount);
		if (mInputs.isKeyHeld(Key::KEY_PAGEDOWN))
			mControlled.move(mControlled.down(), amount);	
	}

	void render() {}

private:
	Inputs &mInputs;
	Transformable &mControlled;
};

class MouseControl
{
public:
	MouseControl(Inputs &inputs, Window &window, Transformable &controlled) : mInputs(inputs), mWindow(window), mControlled(controlled) {}
	virtual ~MouseControl() = default;

	void update(sec t, sec dt)
	{
		const float sensitivity = 0.005f;

		if (mInputs.isMouseReleased(MouseButton::MIDDLE))
		{
			mInputs.showMouseCursor(true);		
		}
		else if (mInputs.isMousePressed(MouseButton::MIDDLE))
		{
			mInputs.showMouseCursor(false);
			mInputs.setMousePosition(mWindow.center());
		}
		else if (mInputs.isMouseHeld(MouseButton::MIDDLE))
		{
	 		auto dpos = mWindow.center() - mInputs.getMousePosition();

			if (dpos.x != 0)
				mControlled.rotate(mControlled.up(), rad{dpos.x * sensitivity});

			if (dpos.y != 0)
				mControlled.rotate(mControlled.right(), rad{dpos.y * sensitivity});

			if (dpos.x != 0 || dpos.y != 0)
				mInputs.setMousePosition(mWindow.center());
		}
	}

	void render() {}

private:
	Inputs &mInputs;
	Window &mWindow;
	Transformable &mControlled;
};

class WindowControl
{
public:	
	WindowControl(Inputs &inputs, Game &game) : mInputs(inputs), mGame(game) {}
	virtual ~WindowControl() = default;

	void init() {}
	void update(sec t, sec dt)
	{
		if (mInputs.isWindowCloseRequested())
			mGame.stop();

		if (mInputs.isKeyHeld(Key::KEY_LEFT_CMD) && mInputs.isKeyPressed(Key::KEY_Z))
			mGame.stop();
	}
	void render();

private:
	Inputs &mInputs;
	Game &mGame;
};




class MyGame : public Game
{
public:

	MyGame(Window &&window) 
		: Game(std::move(window)),
		  mContext(this->window()), 
		  mRenderer(mContext),
		  mWindowControl(this->inputs(), *this)
		   {}

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

		mCameraKeyboardControl.reset(new KeyboardControl(inputs(), mScene.camera()));
		mCameraMouseControl.reset(new MouseControl(inputs(), window(), mScene.camera()));

 	 	mContext.vsync(false);			
	}


	void update(sec t, sec dt)
	{
		mWindowControl.update(t, dt);
		mFPSCounter.update(t, dt);
		mCameraKeyboardControl->update(t, dt);
		mCameraMouseControl->update(t, dt);
	}

	void render()
	{
		mRenderer.render(mScene);
		mFPSCounter.render();
	}


private:	
	Context mContext;
	Renderer mRenderer;
	FPSCounter mFPSCounter;
	std::unique_ptr<KeyboardControl> mCameraKeyboardControl;
	std::unique_ptr<MouseControl> mCameraMouseControl;
	Scene mScene;
	WindowControl mWindowControl;
};


int main(int argc, char **argv)
{
	MyGame(Window(1900, 1200, "MyGame", Window::OPENGL)).start();
	return EXIT_SUCCESS;
}