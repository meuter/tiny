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

}}