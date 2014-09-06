#include <tiny/rendering/shaderprogram.h>
#include <tiny/rendering/shader.h>
#include <tiny/rendering/texture.h>
#include <tiny/rendering/mesh.h>

#include <tiny/core/game.h>
#include <tiny/core/fpscounter.h>
#include <tiny/core/transformable.h>

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
		mMesh          = Mesh::fromFile("res/models/cube.obj");
		mShaderProgram = ShaderProgram::fromFiles("res/shaders/flat_vertex.glsl", "res/shaders/flat_fragment.glsl");
		mTexture       = Texture::fromFile("res/textures/bricks.jpg");
		mPerspective   = projection(toRadian(70), getWindow().aspect(), 0.01f, 1000.0f);
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

	void inputs()
	{
		if (getInputs().isWindowCloseRequested())
			stop();

		if (getInputs().isKeyHeld(Key::KEY_LEFT_CMD) && getInputs().isKeyPressed(Key::KEY_Z))
			stop();
	}

	void update(sec t, sec dt)
	{
		mFPSCounter.update(dt);

 		float sint = sin(rad{t.count()});
		mTransform.setPosition(sint,0,5);
		mTransform.setRotation(vec3(0,1,0), toRadian(sint*180.0f));
	}

	void render()
	{
		mShaderProgram.use();
		mShaderProgram.detectUniform("transform");
		mShaderProgram.setUniform("transform", mPerspective * mTransform.getMatrix());

		mTexture.bind();
		mMesh.draw();
		mFPSCounter.newFrame();
	}

private:	
	ShaderProgram mShaderProgram;
	Texture mTexture;		
	Mesh mMesh;
	FPSCounter mFPSCounter;
	Transformable mTransform;
	mat4 mPerspective;

};


int main(int argc, char **argv)
{
	MyGame(Window(1900, 1200, "MyGame")).start();
	return EXIT_SUCCESS;
}