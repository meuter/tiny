#ifndef __TINY_CORE_CAMERA_H__
#define __TINY_CORE_CAMERA_H__

#include "types.h"

namespace tiny { namespace core {

	class Camera
	{
	public:	
		Camera(const vec3 &position = vec3(0,0,0), const vec3 &forward = vec3(0,0,-1), const vec3 &up = vec3(0,1,0)) 
			: mPosition(position), mForward(normalize(forward)), mUp(normalize(up))	{}

		virtual ~Camera() {}

		inline vec3 up()       const { return mUp; }
		inline vec3 forward()  const { return mForward; }
		inline vec3 left()     const { return normalize(cross(mUp, mForward)); }
		inline vec3 right()    const { return normalize(cross(mForward, mUp)); }


		Camera &move(const vec3 &direction, float amount)
		{
			mPosition += amount * direction;
			return (*this);
		}

		Camera &rotate(vec3 axis, rad angle)
		{
			auto rotation = quat(axis, angle);
			mForward = normalize(rotation.rotate(mForward));
			mUp      = normalize(rotation.rotate(mUp));
			return (*this);
		}

		Camera &roll(rad angle)
		{
			return rotate(mForward, angle);			
		}

		Camera &yaw(rad angle)
		{
			return rotate(mUp, angle);
		}

		Camera &pitch(rad angle)
		{
			return rotate(left(), angle);
		}

		mat4 getMatrix() const 
		{
			return getRotationMatrix() * getTranslationMatrix();
		}

		mat4 getRotationMatrix() const 
		{
			auto mRight = right(); 

			return mat4 
			{
				mRight.x,    mRight.y,    mRight.z,      0.0f,
				mUp.x,       mUp.y,       mUp.z,         0.0f,
				mForward.x,  mForward.y,  mForward.z,    0.0f,
				0.0f,        0.0f,        0.0f,          1.0f
			};
		}

		mat4 getTranslationMatrix() const
		{
			return mat4 
			{
				1.0f, 0.0f, 0.0f, -mPosition.x,
				0.0f, 1.0f, 0.0f, -mPosition.y,
				0.0f, 0.0f, 1.0f, -mPosition.z,
				0.0f, 0.0f, 0.0f, 1.0f,
			};
		}

	private:
		vec3 mPosition, mForward, mUp;
	};

}}

#endif