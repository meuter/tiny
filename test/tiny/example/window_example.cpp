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
		mMesh = Mesh::fromFile("res/models/cube.obj");
		mShaderProgram = ShaderProgram::fromFiles("res/shaders/flat_vertex.glsl", "res/shaders/flat_fragment.glsl");
		mTexture = Texture::fromFile("res/textures/bricks.jpg");
		getWindow().vsync(false);		
	}

	void update(sec t, sec dt)
	{
		mFPSCounter.update(dt);

 		float sint = sin(rad{t.count()});
		mTransform.setPosition(sint,0,0);
		mTransform.setRotation(vec3(0,1,0), toRadian(sint*180.0f));
		mTransform.setScale(0.7f * sint, 0.7f * sint, 0.7f * sint);			
	}

	void inputs()
	{
		if (getInputs().isWindowCloseRequested())
			stop();

		if (getInputs().isKeyHeld(Key::KEY_LEFT_CMD) && getInputs().isKeyPressed(Key::KEY_Z))
			stop();
	}

	void render()
	{
		mShaderProgram.use();

		mShaderProgram.detectUniform("transform");
		mShaderProgram.setUniform("transform", mTransform.getMatrix());

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
};


int main(int argc, char **argv)
{
	MyGame(Window(1900, 1200, "MyGame")).start();
	return EXIT_SUCCESS;
}