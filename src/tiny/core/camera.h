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

		inline void roll(rad angle)  { rotate(forward(), angle); }
		inline void yaw(rad angle)   { rotate(up(), angle); }
		inline void pitch(rad angle) { rotate(right(), angle); }

		mat4 getViewMatrix() const 
		{
			mat4 reverseTranslation = 
			{
				1.0f, 0.0f, 0.0f, -mPosition.x,
				0.0f, 1.0f, 0.0f, -mPosition.y,
				0.0f, 0.0f, 1.0f, -mPosition.z,
				0.0f, 0.0f, 0.0f, 1.0f,
			};

			return getRotationMatrix() * reverseTranslation;
		}
	};

}}

#endif