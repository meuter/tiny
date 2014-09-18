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
	vec3 newForward = normalize(target - mPosition);
	vec3 newLeft    = normalize(cross(up(), newForward));
	vec3 newUp      = normalize(cross(newForward, newLeft));

	mRotation = quat(newForward, newUp);
}

const mat4 &Camera::projectionMatrix() const
{
	return mProjection; 
}

mat4 Camera::viewMatrix() const 
{
	Transformable reverse;

	reverse.rotateTo(mRotation.conjugate());
	reverse.moveTo(-mPosition);

	return reverse.rotationMatrix() * reverse.translationMatrix();
}
}}