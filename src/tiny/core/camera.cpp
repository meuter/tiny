#include "camera.h"

namespace tiny { namespace core {

Camera Camera::withPerspective(rad fieldOfView, float aspectRatio, float zNear, float zFar)
{
	float zRange = zNear - zFar;
	float yScale = 1.0f / tan(fieldOfView/2.0);
	float xScale = -1 * yScale / aspectRatio;
	float zScale = -1 * (zNear+zFar)/zRange;

	return Camera({
		xScale, 0.0f,   0.0f,    0.0f,
		0.0f,   yScale, 0.0f,    0.0f,
		0.0f,   0.0f,   zScale,  2.0f*zFar*zNear/zRange,
		0.0f,   0.0f,   1.0f,    0.0f
	});
}


void Camera::lookAt(float x, float y, float z)
{
	lookAt(vec3(x,y,z));
}

void Camera::lookAt(const vec3 target) 
{
	// FIXME: probably not working as intended
	vec3 newForward = normalize(target - position());
	vec3 newLeft    = normalize(cross(up(), newForward));
	vec3 newUp      = normalize(cross(newForward, newLeft));

	rotateTo(quat(newForward, newUp));
}

const mat4 &Camera::projectionMatrix() const
{
	return mProjection; 
}

mat4 Camera::viewMatrix() const 
{
	Transformable reverse;

	reverse.rotateTo(rotation().conjugate());
	reverse.moveTo(-position());

	return reverse.rotationMatrix() * reverse.translationMatrix();
}

void Camera::update(Window &window, Inputs &inputs, sec dt)
{

	{
		float amount = dt.count() * 10;

		if (inputs.isKeyHeld(Key::KEY_UP))
			move(forward(), amount);
		if (inputs.isKeyHeld(Key::KEY_DOWN))
			move(backward(), amount);
		if (inputs.isKeyHeld(Key::KEY_LEFT))
			move(left(), amount);
		if (inputs.isKeyHeld(Key::KEY_RIGHT))
			move(right(), amount);
		if (inputs.isKeyHeld(Key::KEY_PAGEUP))
			move(up(), amount);
		if (inputs.isKeyHeld(Key::KEY_PAGEDOWN))
			move(down(), amount);
	}


	{
		const float sensitivity = 0.005f;

		if (inputs.isMouseReleased(MouseButton::MIDDLE))
		{
			inputs.showMouseCursor(true);		
		}
		else if (inputs.isMousePressed(MouseButton::MIDDLE))
		{
			inputs.showMouseCursor(false);
			inputs.setMousePosition(window.center());
		}
		else if (inputs.isMouseHeld(MouseButton::MIDDLE))
		{
	 		auto dpos = window.center() - inputs.getMousePosition();

			if (dpos.x != 0)
				rotate(up(), rad{dpos.x * sensitivity});

			if (dpos.y != 0)
				rotate(right(), rad{dpos.y * sensitivity});

			if (dpos.x != 0 || dpos.y != 0)
				inputs.setMousePosition(window.center());
		}
	}

}



}}