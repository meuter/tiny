#include <tiny/rendering/shaderprogram.h>
#include <tiny/rendering/shader.h>
#include <tiny/rendering/texture.h>
#include <tiny/rendering/mesh.h>

#include <tiny/core/game.h>
#include <tiny/core/fpscounter.h>

using namespace tiny::rendering;
using namespace tiny::core;

class MyGame : public Game
{
public:

	MyGame(Window &&window) : Game(std::move(window))
	{			
	}

	void init()
	{
		mShaderProgram = ShaderProgram::fromFiles("res/shaders/flat_vertex.glsl", "res/shaders/flat_fragment.glsl");
		mTexture = Texture::fromFile("res/textures/bricks.jpg");
		getWindow().vsync(false);		
	}

	void update(sec dt)
	{
		mFPSCounter.update(dt);
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
		mTexture.bind();
		mMesh.draw();
		mFPSCounter.newFrame();
	}

private:	
	ShaderProgram mShaderProgram;
	Texture mTexture;		
	Mesh mMesh;
	FPSCounter mFPSCounter;
};


int main(int argc, char **argv)
{
	MyGame(Window(1080, 768, "MyGame")).start();
	return EXIT_SUCCESS;
}