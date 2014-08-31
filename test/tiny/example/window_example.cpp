#include <tiny/rendering/window.h>
#include <tiny/rendering/shaderprogram.h>
#include <tiny/rendering/shader.h>
#include <tiny/rendering/texture.h>
#include <tiny/rendering/mesh.h>
#include <tiny/core/types.h>
#include <tiny/core/inputs.h>
#include <tiny/core/keys.h>
#include <tiny/core/engine.h>
#include <iostream>

using namespace tiny::rendering;
using namespace tiny::core;

class MyGame
{
public:

	void init(Engine &engine)
	{
		mShaderProgram = ShaderProgram::fromFiles("res/shaders/flat_vertex.glsl", "res/shaders/flat_fragment.glsl");
		mTexture = Texture::fromFile("res/textures/bricks.jpg");
		engine.window().vsync(false);
	}

	void inputs(Engine &engine)
	{
		Inputs &inputs = engine.inputs();

		if (inputs.isWindowCloseRequested())
			engine.stop();

		if (inputs.isKeyHeld(Key::KEY_LEFT_CMD) && inputs.isKeyPressed(Key::KEY_Z))
			engine.stop();
	}


	void update(Engine &engine, sec dt)
	{
		sinceLastFrame += dt;
		if( sinceLastFrame > sec(1))
		{
			std::cout << nFrames << " FPS" << std::endl;
			nFrames = 0;
			sinceLastFrame = sec(0);
		}
	}

	void render(Engine &engine)
	{
		engine.window().clear();
		mShaderProgram.use();
		mMesh.draw();
		nFrames++;
	}

private:	
	ShaderProgram mShaderProgram;
	Texture mTexture;		
	Mesh mMesh;
	int nFrames;
	sec sinceLastFrame;
};

int main(int argc, char **argv)
{
	Engine(Window(1080, 768, "GameEngine")).play(MyGame());
	return EXIT_SUCCESS;
}