#ifndef __TINY_CORE_CAMERA_H__
#define __TINY_CORE_CAMERA_H__

#include "types.h"

namespace tiny { namespace core {

	class Camera
	{
	public:	
		Camera(const vec3 &position = vec3(0,0,0), const quat &rotation = quat(0,0,0,1)) 
			: mPosition(position), mRotation(rotation)	{}

		virtual ~Camera() {}

		inline vec3 right()    const { return mRotation.right(); }
		inline vec3 left()     const { return mRotation.left(); }
		inline vec3 up()       const { return mRotation.up(); }
		inline vec3 down()     const { return mRotation.down(); }
		inline vec3 forward()  const { return mRotation.forward(); }
		inline vec3 backward() const { return mRotation.backward(); }

		void move(const vec3 &direction, float amount)
		{
			mPosition += amount * direction;
		}

		void rotate(const quat &rotation)
		{
			mRotation = normalize(rotation * mRotation);
		}

		void rotate(const vec3 &axis, rad angle)
		{
			rotate(quat(axis, angle));
		}

		void roll(rad angle)
		{
			rotate(mRotation.forward(), angle);
		}

		void yaw(rad angle)
		{
		 	rotate(mRotation.up(), angle);
		}

		void pitch(rad angle)
		{
			rotate(mRotation.right(), angle);
		}

		mat4 getMatrix() const 
		{
			return getRotationMatrix() * getTranslationMatrix();
		}

		mat4 getRotationMatrix() const 
		{
			auto mRight   = mRotation.right(); 
			auto mUp      = mRotation.up();
			auto mForward = mRotation.forward();

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
		vec3 mPosition;
		quat mRotation;
	};

}}

#endif