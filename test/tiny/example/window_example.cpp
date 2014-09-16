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


class PhongShaderProgram : public ShaderProgram
{
public:
	PhongShaderProgram() : ShaderProgram(ShaderProgram::fromFiles("res/shaders/phong.vs", "res/shaders/phong.fs")) {}

	using ShaderProgram::setUniform;

	void setUniform(const std::string &uniform, const BaseLight &lightSource)
	{
		setUniform(uniform + ".color",     lightSource.color);
		setUniform(uniform + ".intensity", lightSource.intensity);
	}

	void setUniform(const std::string &uniform, const DirectionalLight &directionalLight)
	{
		setUniform(uniform + ".base",    dynamic_cast<const BaseLight&>(directionalLight));
		setUniform(uniform + ".direction", directionalLight.direction);
	}
};


class MyGame : public Game
{
public:

	MyGame(Window &&window) : Game(std::move(window)) {	}

	void init()
	{
		mMesh             = Mesh::fromFile("res/models/box_full.obj");
		mTexture          = Texture::fromFile("res/textures/bricks.jpg");
		mCamera           = Camera::withPerspective(toRadian(70), window().aspect(), 0.01f, 1000.0f);
		mMouseLocked      = false;
		mDirectionalLight = DirectionalLight(vec3(1,1,1)*0.7f, 1.0f, vec3(0,-1,0));

		mSpecularIntensity = 2;
		mSpecularExponent = 32;

		mCamera.moveTo(0,0,5);
		mCamera.lookAt(0,0,0);
		
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

		if (!inputs().isKeyHeld(Key::KEY_SPACE))
		{
 			float sint = sin(rad{t.count()});
			mTransform.moveTo(sint,0,0);
			mTransform.rotateTo(vec3(0,1,0), toRadian(sint*180.0f));
		}

		move(dt);
		look(dt);
	}

	void render()
	{
		mShaderProgram.use();
		mShaderProgram.setUniform("MVP", mCamera.getViewProjection() * mTransform.getModel());
		mShaderProgram.setUniform("M", mTransform.getModel());
		mShaderProgram.setUniform("ambient", vec3(0.2f,0.2f,0.2f));
		mShaderProgram.setUniform("directionalLight", mDirectionalLight);
		mShaderProgram.setUniform("specularIntensity", mSpecularIntensity);
		mShaderProgram.setUniform("specularExponent", mSpecularExponent);
		mShaderProgram.setUniform("eyePos", mCamera.position());

		mTexture.bind();
		mMesh.draw();
		mFPSCounter.newFrame();
	}

private:	
	PhongShaderProgram mShaderProgram;
	Texture mTexture;	
	float mSpecularIntensity, mSpecularExponent;	
	Mesh mMesh;
	Camera mCamera; 
	FPSCounter mFPSCounter;
	Transformable mTransform;
	DirectionalLight mDirectionalLight;
	bool mMouseLocked;
};


int main(int argc, char **argv)
{
	MyGame(Window(1900, 1200, "MyGame")).start();
	return EXIT_SUCCESS;
}