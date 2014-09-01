#include <tiny/rendering/shaderprogram.h>
#include <tiny/rendering/shader.h>
#include <tiny/rendering/texture.h>
#include <tiny/rendering/mesh.h>

#include <tiny/core/game.h>
#include <tiny/core/fpscounter.h>

#include <iostream>

using namespace tiny::rendering;
using namespace tiny::core;
using namespace tiny::math;


class MyGame : public Game
{
public:

	MyGame(Window &&window) : Game(std::move(window))
	{		
	}

	void init()
	{
		std::vector<Mesh::vertex> vertices = 
		{
			{ vec3(-1.0, -1.0, 0.0) },
			{ vec3( 0.0,  1.0, 0.0) },
			{ vec3( 1.0, -1.0, 0.0) },
		};

		mMesh.loadVertices(vertices);

		mShaderProgram = ShaderProgram::fromFiles("res/shaders/flat_vertex.glsl", "res/shaders/flat_fragment.glsl");
		mTexture = Texture::fromFile("res/textures/bricks.jpg");
		getWindow().vsync(false);	
		mScale = 1.0f;	
		t = sec{0};
		mColor = vec4(1,1,1,1);
	}

	void update(sec dt)
	{
		t += dt;

		mFPSCounter.update(dt);

 		mScale = cos(rad{t.count()});
 		mColor *= 0.9998f;

 		std::cout << mScale << std::endl;

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

		mShaderProgram.detectUniform("scale");
		mShaderProgram.setUniform("scale", mScale);

		mShaderProgram.detectUniform("color");
		mShaderProgram.setUniform("color", mColor);

		mTexture.bind();
		mMesh.draw();
		mFPSCounter.newFrame();
	}

private:	
	ShaderProgram mShaderProgram;
	Texture mTexture;		
	Mesh mMesh;
	FPSCounter mFPSCounter;

	float mScale;
	vec4 mColor;
	sec t;
};


int main(int argc, char **argv)
{
	MyGame(Window(1080, 768, "MyGame")).start();
	return EXIT_SUCCESS;
}