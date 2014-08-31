#include <iostream>
#include <tiny/rendering/window.h>
#include <tiny/rendering/shaderprogram.h>
#include <tiny/rendering/shader.h>
#include <tiny/rendering/texture.h>
#include <tiny/core/types.h>
#include <tiny/core/inputs.h>
#include <tiny/core/keys.h>
#include <tiny/core/engine.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <vector>

#include <iostream>

using tiny::rendering::Window;
using tiny::rendering::ShaderProgram;
using tiny::rendering::Shader;
using tiny::rendering::Texture;
using tiny::core::vec3;
using tiny::core::Engine;
using tiny::core::Inputs;
using tiny::core::Key;
using tiny::core::MouseButton;
using tiny::core::sec;

#define NUM_BUFFERS 1

struct Mesh
{
	Mesh()
	{
		vertices.push_back(vec3(-0.5, -0.5, 0));
		vertices.push_back(vec3( 0,   0.5,  0));
		vertices.push_back(vec3(0.5,-0.5, 0));

		glGenVertexArrays(1, &vertexArrayHandle);
		glBindVertexArray(vertexArrayHandle);

		glGenBuffers(NUM_BUFFERS, vertexArrayBufferHandles);
		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBufferHandles[0]);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 0,0);

		glBindVertexArray(0);
	}

	~Mesh()
	{
		glDeleteVertexArrays(1, &vertexArrayHandle);
	}

	void draw()
	{
		glBindVertexArray(vertexArrayHandle);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);

	}


	GLuint vertexArrayHandle;
	GLuint vertexArrayBufferHandles[NUM_BUFFERS];
	std::vector<vec3> vertices;
};


class MyGame
{
public:

	void init(Engine &engine)
	{
		mShaderProgram = ShaderProgram::fromFiles("res/shaders/flat_vertex.glsl", "res/shaders/flat_fragment.glsl");
		mTexture = Texture::fromFile("res/textures/bricks.jpg");

		SDL_GL_SetSwapInterval(0);
		glClearColor(0,0,0.5,1);
	}

	void inputs(Engine &engine, Inputs &inputs)
	{
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
		glClear(GL_COLOR_BUFFER_BIT);

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