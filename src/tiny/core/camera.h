#ifndef __TINY_CORE_CAMERA_H__
#define __TINY_CORE_CAMERA_H__

#include "types.h"
#include "transformable.h"
#include <algorithm>

namespace tiny { namespace core {

	class Camera : public Transformable
	{
	public:	

		static Camera withPerspective(rad fieldOfView, float aspectRatio, float zNear, float zFar)
		{
			float zRange = zFar - zNear;  
			float tanHalfFov = tan(fieldOfView/2.0);

			return Camera({
				1.0f/(tanHalfFov*aspectRatio), 0.0f,            0.0f,                 0.0f,
				0.0f,                          1.0f/tanHalfFov, 0.0f,                 0.0f,
				0.0f,                          0.0f,            (-zNear-zFar)/zRange, 2.0f*zFar*zNear/zRange,
				0.0f,                          0.0f,            1.0f,                 0.0f
			});
		}

		Camera() {}
		Camera(const mat4 &projection) : mProjection(projection) {}

		virtual ~Camera() {}

		inline void roll(rad angle)  { rotate(forward(), angle); }
		inline void yaw(rad angle)   { rotate(up(),      angle); }
		inline void pitch(rad angle) { rotate(right(),   angle); }

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

		inline const mat4 &projection() const { return mProjection; }

		mat4 getViewMatrix() const 
		{
			Transformable reverse;

			reverse.rotateTo(mRotation.conjugate());
			reverse.moveTo(-mPosition);

			return reverse.getRotationMatrix() * reverse.getTranslationMatrix();
		}

		mat4 getViewProjection() const
		{
			return projection() * getViewMatrix();
		}

	private:
		mat4 mProjection;
	};

}}

#endif