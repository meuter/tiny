#include <tiny/rendering/shaderprogram.h>
#include <tiny/rendering/shader.h>
#include <tiny/rendering/mesh.h>
#include <tiny/rendering/material.h>

#include <tiny/core/game.h>
#include <tiny/core/fpscounter.h>
#include <tiny/core/transformable.h>
#include <tiny/core/camera.h>

#include <iostream>

using namespace tiny::rendering;
using namespace tiny::core;
using namespace tiny::math;

struct BaseLight
{
	BaseLight() : color(0,0,0), intensity(0) {}
	BaseLight(const vec3 &color, const float &intensity) : color(color), intensity(intensity) {}
	vec3 color;
	float intensity;
};

struct DirectionalLight : public BaseLight
{
	DirectionalLight() : BaseLight(), direction(0,0,0) {}
	DirectionalLight(const vec3 &color, const float &intensity, const vec3 &direction) : BaseLight(color, intensity), direction(direction) {}
	vec3 direction;
};

class BasicShader : public ShaderProgram
{
public:
	BasicShader() : ShaderProgram(ShaderProgram::fromFiles("res/shaders/basic.vs", "res/shaders/basic.fs")) {}

	void draw(const Camera &camera, const Mesh &mesh)
	{
		use();

		setUniform("M", mesh.modelMatrix());
		setUniform("V", camera.viewMatrix());
		setUniform("P", camera.projectionMatrix());
		setUniform("texture", 0);
		setUniform("diffuse", mesh.material().diffuse());

		mesh.material().texture().bind(0);
		mesh.draw();
	}

	void setDirectionalLight(const DirectionalLight directional)
	{
	}

	void setSpecularLight(float intensity, float exponent)
	{
	}
};


class PhongShaderProgram : public ShaderProgram
{
public:
	PhongShaderProgram() : ShaderProgram(ShaderProgram::fromFiles("res/shaders/phong.vs", "res/shaders/phong.fs")) {}

	using ShaderProgram::setUniform;
	using ShaderProgram::use;

	void setUniform(const std::string &uniform, const BaseLight &lightSource)
	{
		setUniform(uniform + ".color",     lightSource.color);
		setUniform(uniform + ".intensity", lightSource.intensity);
	}

	void setUniform(const std::string &uniform, const DirectionalLight &directionalLight)
	{
		setUniform(uniform + ".base",     dynamic_cast<const BaseLight&>(directionalLight));
		setUniform(uniform + ".direction", directionalLight.direction);
	}

	void setUniform(const std::string &uniform, const Material &material)
	{
		const unsigned int textureSlot = 1;

		setUniform(uniform + ".texture", textureSlot);
		setUniform(uniform + ".ambient", material.ambient());
		setUniform(uniform + ".diffuse", material.diffuse());

		material.texture().bind(textureSlot);
	}

	void setDirectionalLight(const DirectionalLight directional)
	{
		mDirectional = directional;
	}

	void setSpecularLight(float intensity, float exponent)
	{
		mSpecularIntensity = intensity;
		mSpecularExponent = exponent;
	}

	void draw(const Camera &camera, const Mesh &mesh)
	{
		use();
		setUniform("M", mesh.modelMatrix());
		setUniform("V", camera.viewMatrix());
		setUniform("P", camera.projectionMatrix());
		setUniform("material", mesh.material());
		setUniform("directionalLight", mDirectional);
		setUniform("specularIntensity", mSpecularIntensity);
		setUniform("specularExponent", mSpecularExponent);
		setUniform("eyePos", camera.position());

		mesh.draw();
	}
private:
	DirectionalLight mDirectional;
	float mSpecularIntensity, mSpecularExponent;
};


class MyGame : public Game
{
public:

	MyGame(Window &&window) : Game(std::move(window)) {	}

	void init()
	{
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);


		Mesh ground, box, sphere;

		ground = Mesh::fromFiles("res/models/ground.obj", "res/models/ground.mtl");
		ground.moveTo(0,-2,0);
		mMeshes.push_back(std::move(ground));

		box = Mesh::fromFiles("res/models/box.obj", "res/models/box.mtl");
		box.moveTo(0,4,0);
		mMeshes.push_back(std::move(box));

		sphere = Mesh::fromFiles("res/models/sphere_smooth.obj", "res/models/sphere_smooth.mtl");
		mMeshes.push_back(std::move(sphere));

		mCamera             = Camera::withPerspective(toRadian(70), window().aspect(), 0.01f, 1000.0f);
		mMouseLocked        = false;

		mShaderProgram.setDirectionalLight(DirectionalLight(vec3(1,1,1)*0.7f, 1.0f, vec3(0,-1,0)));
		mShaderProgram.setSpecularLight(2,32);

		mCamera.moveTo(0,0,5);
		mCamera.rotateTo(Transformable::Y_AXIS, toRadian(180));

 		window().vsync(false);				
	}


	void update(sec t, sec dt)
	{
		if (shouldStop())
			stop();

		mFPSCounter.update(dt);

		moveCamera(dt);
		lookCamera(dt);
	}

	void render()
	{
		for (auto &mesh: mMeshes)
			mShaderProgram.draw(mCamera, mesh);
		mFPSCounter.newFrame();
	}


	bool shouldStop()
	{
		if (inputs().isWindowCloseRequested())
			return true;

		if (inputs().isKeyHeld(Key::KEY_LEFT_CMD) && inputs().isKeyPressed(Key::KEY_Z))
			return true;

		return false;
	}

	void moveCamera(sec dt)
	{
		float amount = dt.count() * 10;

		if (inputs().isKeyHeld(Key::KEY_UP))
			mCamera.move(mCamera.forward(), amount);
		if (inputs().isKeyHeld(Key::KEY_DOWN))
			mCamera.move(mCamera.backward(), amount);
		if (inputs().isKeyHeld(Key::KEY_LEFT))
			mCamera.move(mCamera.left(), amount);
		if (inputs().isKeyHeld(Key::KEY_RIGHT))
			mCamera.move(mCamera.right(), amount);
		if (inputs().isKeyHeld(Key::KEY_PAGEUP))
			mCamera.move(mCamera.up(), amount);
		if (inputs().isKeyHeld(Key::KEY_PAGEDOWN))
			mCamera.move(mCamera.down(), amount);
	}

	void lookCamera(sec dt)
	{
		const float sensitivity = 0.005f;

		if (inputs().isKeyPressed(Key::KEY_ESCAPE))
		{
			inputs().showMouseCursor(true);
			mMouseLocked = false;
		}

		if (mMouseLocked)
		{
	 		auto dpos = window().center() - inputs().getMousePosition();

			if (dpos.x != 0)
				mCamera.rotate(mCamera.up(), rad{dpos.x * sensitivity});

			if (dpos.y != 0)
				mCamera.rotate(mCamera.right(), rad{dpos.y * sensitivity});

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

private:	
	PhongShaderProgram mShaderProgram;
	std::vector<Mesh> mMeshes;
	Camera mCamera; 
	FPSCounter mFPSCounter;
	bool mMouseLocked;
};


int main(int argc, char **argv)
{
	MyGame(Window(1900, 1200, "MyGame")).start();
	return EXIT_SUCCESS;
}