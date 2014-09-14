#ifndef __TINY_CORE_CAMERA_H__
#define __TINY_CORE_CAMERA_H__

#include "types.h"
#include "transformable.h"

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

		mat4 getView() const 
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

		mat4 getProjection() const
		{
			return mProjection;
		}

		mat4 getViewProjection() const
		{
			return getProjection() * getView();
		}

	private:
		mat4 mProjection;
	};

}}

#endif