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


class MyGame : public Game
{
public:

	MyGame(Window &&window) : Game(std::move(window)) {	}

	void init()
	{
		mMesh          = Mesh::fromFile("res/models/box.obj");
		mMesh2         = Mesh::fromFile("res/models/box.obj");
		mShaderProgram = ShaderProgram::fromFiles("res/shaders/flat_vertex.glsl", "res/shaders/flat_fragment.glsl");
		mTexture       = Texture::fromFile("res/textures/bricks.jpg");
		mProjection    = projection(toRadian(70), window().aspect(), 0.01f, 1000.0f);
		mMouseLocked   = false;
		mWindowCenter  = window().center();

		mCamera.move(mCamera.forward(), -5);
 		window().vsync(false);				
	}

	mat4 projection(rad fieldOfView, float aspectRatio, float zNear, float zFar)
	{
		float zRange = zFar - zNear;  
		float tanHalfFov = tan(fieldOfView/2.0);

		return mat4{
			1.0f/(tanHalfFov*aspectRatio), 0.0f,            0.0f,                 0.0f,
			0.0f,                          1.0f/tanHalfFov, 0.0f,                 0.0f,
			0.0f,                          0.0f,            (-zNear-zFar)/zRange, 2.0f*zFar*zNear/zRange,
			0.0f,                          0.0f,            1.0f,                 0.0f
		};
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

		move(dt);
		look(dt);
	}

	void render()
	{
		auto view = mCamera.getMatrix();

		mShaderProgram.use();
		mShaderProgram.detectUniform("transform");
		mShaderProgram.setUniform("transform", mProjection * view * mTransform.getMatrix());

		mTexture.bind();
		mMesh.draw();
		mFPSCounter.newFrame();
	}

private:	
	ShaderProgram mShaderProgram;
	Texture mTexture;		
	Mesh mMesh, mMesh2;
	Camera mCamera; 
	FPSCounter mFPSCounter;
	Transformable mTransform;
	mat4 mProjection;
	bool mMouseLocked;
	ivec2 mWindowCenter;
};


int main(int argc, char **argv)
{
	MyGame(Window(1900, 1200, "MyGame")).start();
	return EXIT_SUCCESS;
}