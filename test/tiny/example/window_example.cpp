#include <tiny/rendering/shaderprogram.h>
#include <tiny/rendering/shader.h>
#include <tiny/rendering/texture.h>
#include <tiny/rendering/mesh.h>

#include <tiny/core/game.h>
#include <tiny/core/fpscounter.h>
#include <tiny/core/transformable.h>
#include <tiny/core/camera.h>

#include <iostream>

using namespace tiny::rendering;
using namespace tiny::core;
using namespace tiny::math;


class BasicShaderProgram : public ShaderProgram
{
public:
	BasicShaderProgram() : ShaderProgram(ShaderProgram::fromFiles("res/shaders/basic.vs", "res/shaders/basic.fs"))
	{
		detectUniform("MVP");		
	}

	void setMVP(const mat4 &MVP)
	{
		setUniform("MVP", MVP);
	}
};

class PhongShaderProgram : public ShaderProgram
{
public:
	PhongShaderProgram() : ShaderProgram(ShaderProgram::fromFiles("res/shaders/phong.vs", "res/shaders/phong.fs"))
	{
		detectUniform("MVP");		
		detectUniform("ambient");
	}

	void setMVP(const mat4 &MVP)
	{
		setUniform("MVP", MVP);
	}

	void setAmbientLight(const vec3 &ambientLight)
	{
		setUniform("ambient", ambientLight);
	}
};


class MyGame : public Game
{
public:

	MyGame(Window &&window) : Game(std::move(window)) {	}

	void init()
	{
		mMesh          = Mesh::fromFile("res/models/box.obj");
		mTexture       = Texture::fromFile("res/textures/bricks.jpg");
		mCamera        = Camera::withPerspective(toRadian(70), window().aspect(), 0.01f, 1000.0f);
		mMouseLocked   = false;

		mCamera.move(mCamera.forward(), -5);
 		window().vsync(false);				
	}

	bool shouldStop()
	{
		if (inputs().isWindowCloseRequested())
			return true;

		if (inputs().isKeyHeld(Key::KEY_LEFT_CMD) && inputs().isKeyPressed(Key::KEY_Z))
			return true;

		return false;
	}

	void move(sec dt)
	{
		float amount = dt.count() * 10;

		if (inputs().isKeyHeld(Key::KEY_UP))
			mCamera.move(mCamera.forward(), amount);
		if (inputs().isKeyHeld(Key::KEY_DOWN))
			mCamera.move(mCamera.forward(), -amount);
		if (inputs().isKeyHeld(Key::KEY_LEFT))
			mCamera.move(mCamera.left(), amount);
		if (inputs().isKeyHeld(Key::KEY_RIGHT))
			mCamera.move(mCamera.right(), amount);
	}

	void look(sec dt)
	{
		const float sensitivity = 0.005f;

		if (inputs().isKeyPressed(Key::KEY_ESCAPE))
		{
			inputs().showMouseCursor(true);
			mMouseLocked = false;
		}

		if (mMouseLocked)
		{
	 		auto dpos = inputs().getMousePosition() - window().center();

			if (dpos.x != 0)
				mCamera.yaw(rad{dpos.x * sensitivity});

			if (dpos.y != 0)
				mCamera.pitch(rad{dpos.y * sensitivity});

			if (dpos.x != 0 || dpos.y != 0)
				inputs().setMousePosition(window().center());
		}

		if (inputs().isMousePressed(MouseButton::LEFT))
		{
			inputs().showMouseCursor(false);
			inputs().setMousePosition(window().center());
			mMouseLocked = true;
		}
	}

	void update(sec t, sec dt)
	{
		if (shouldStop())
			stop();

		mFPSCounter.update(dt);

 		float sint = sin(rad{t.count()});
		mTransform.moveTo(sint,0,5);
		mTransform.rotateTo(vec3(0,1,0), toRadian(sint*180.0f));

		move(dt);
		look(dt);
	}

	void render()
	{
		mShaderProgram.use();
		mShaderProgram.setMVP(mCamera.getViewProjection() * mTransform.getModel());
		mShaderProgram.setAmbientLight(vec3(0.2f,0.2f,0.2f));

		mTexture.bind();
		mMesh.draw();
		mFPSCounter.newFrame();
	}

private:	
	PhongShaderProgram mShaderProgram;
	Texture mTexture;		
	Mesh mMesh;
	Camera mCamera; 
	FPSCounter mFPSCounter;
	Transformable mTransform;
	bool mMouseLocked;
};


int main(int argc, char **argv)
{
	MyGame(Window(1900, 1200, "MyGame")).start();
	return EXIT_SUCCESS;
}