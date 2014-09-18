#ifndef __TINY_CORE_CAMERA_H__
#define __TINY_CORE_CAMERA_H__

#include "types.h"
#include "transformable.h"
#include <tiny/math/trigo.h>
#include <algorithm>

namespace tiny { namespace core {

	class Camera : public Transformable
	{
	public:	

		static Camera withPerspective(rad fieldOfView, float aspectRatio, float zNear, float zFar)
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

		Camera() {}
		Camera(const mat4 &projection) : mProjection(projection) {}

		virtual ~Camera() {}

		inline void lookAt(float x, float y, float z)
		{
			lookAt(vec3(x,y,z));
		}

		inline void lookAt(const vec3 target) 
		{
			vec3 newForward = normalize(target - mPosition);
			vec3 newLeft    = normalize(cross(up(), newForward));
			vec3 newUp      = normalize(cross(newForward, newLeft));

			mRotation = quat(newForward, newUp);
		}

		inline const mat4 &projectionMatrix() const
		{
			return mProjection; 
		}

		mat4 viewMatrix() const 
		{
			Transformable reverse;

			reverse.rotateTo(mRotation.conjugate());
			reverse.moveTo(-mPosition);

			return reverse.rotationMatrix() * reverse.translationMatrix();
		}

	private:
		mat4 mProjection;
	};

}}

#endif