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
		mShaderProgram = ShaderProgram::fromFiles("res/shaders/flat_vertex.glsl", "res/shaders/flat_fragment.glsl");
		mTexture       = Texture::fromFile("res/textures/bricks.jpg");
		mProjection    = projection(toRadian(70), getWindow().aspect(), 0.01f, 1000.0f);

		std::cout << "Matrix = \n" << mCamera.getMatrix() << std::endl;

		std::cout << "Up     = " << mCamera.up() << std::endl;
		std::cout << "Fwd    = " << mCamera.forward() << std::endl;
		std::cout << "Left   = " << mCamera.left() << std::endl;
		std::cout << "Right  = " << mCamera.right() << std::endl;

		mCamera.move(mCamera.forward(), -5);

		getWindow().vsync(false);				
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

	void update(sec t, sec dt)
	{
		if (getInputs().isWindowCloseRequested())
			stop();

		if (getInputs().isKeyHeld(Key::KEY_LEFT_CMD) && getInputs().isKeyPressed(Key::KEY_Z))
			stop();

		mFPSCounter.update(dt);

 	// 	float sint = sin(rad{t.count()});
		mTransform.setPosition(0,0,0);
		// mTransform.setRotation(vec3(0,1,0), toRadian(sint*180.0f));

		float moveAmount = dt.count() * 10;
		rad rotationAmount = rad{dt.count()* 1.0};

		if (getInputs().isKeyHeld(Key::KEY_W))
			mCamera.move(mCamera.forward(), moveAmount);
		if (getInputs().isKeyHeld(Key::KEY_S))
			mCamera.move(mCamera.forward(), -moveAmount);
		if (getInputs().isKeyHeld(Key::KEY_A))
			mCamera.move(mCamera.left(), moveAmount);
		if (getInputs().isKeyHeld(Key::KEY_D))
			mCamera.move(mCamera.right(), moveAmount);

		if (getInputs().isKeyHeld(Key::KEY_UP))
			mCamera.pitch(rotationAmount);
		if (getInputs().isKeyHeld(Key::KEY_DOWN))
			mCamera.pitch(-rotationAmount);
		if (getInputs().isKeyHeld(Key::KEY_LEFT))
			mCamera.yaw(-rotationAmount);
		if (getInputs().isKeyHeld(Key::KEY_RIGHT))
			mCamera.yaw(rotationAmount);

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
	Mesh mMesh;
	Camera mCamera; 
	FPSCounter mFPSCounter;
	Transformable mTransform;
	mat4 mProjection;

};


int main(int argc, char **argv)
{
	MyGame(Window(1900, 1200, "MyGame")).start();
	return EXIT_SUCCESS;
}