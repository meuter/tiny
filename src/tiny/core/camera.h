#ifndef __TINY_CORE_CAMERA_H__
#define __TINY_CORE_CAMERA_H__

#include "types.h"
#include "transformable.h"

namespace tiny { namespace core {

	class Camera : public Transformable
	{
	public:	
		Camera() {}
		virtual ~Camera() {}

		mat4 getMatrix() const 
		{
			return getRotationMatrix() * getTranslationMatrix();
		}

		mat4 getRotationMatrix() const 
		{
			auto mRight   = right(); 
			auto mUp      = up();
			auto mForward = forward();

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
	};

}}

#endif