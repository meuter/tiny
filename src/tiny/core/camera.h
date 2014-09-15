#ifndef __TINY_CORE_CAMERA_H__
#define __TINY_CORE_CAMERA_H__

#include "types.h"
#include "transformable.h"

#include <iostream>

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

		inline void lookAt(vec3 target) 
		{
			vec3 newForward = normalize(target - mPosition);
			vec3 newRight   = normalize(cross(up(), newForward));
			vec3 newUp      = normalize(cross(newForward, newRight));

			mat4 m =
			{
				newRight.x,   newRight.y,   newRight.z,   0.0f,
				newUp.x,      newUp.y,      newUp.z,      0.0f,
				newForward.x, newForward.y, newForward.z, 0.0f,
				0.0f,         0.0f,         0.0f,         1.0f
			};

			float trace = m(0,0) + m(1,1) + m(2,2);

			if (trace > 0) 
			{ 
				float S = sqrt(trace+1.0) * 2;
				mRotation = quat(
					(m(2,1) - m(1,2)) / S,
					(m(0,2) - m(2,0)) / S,
					(m(1,0) - m(0,1)) / S,
					0.25 * S
				);
			}
			else if (m(0,0) > m(1,1) && m(0,0) > m(2,2)) 
			{ 
				float S = sqrt(1.0 + m(0,0) - m(1,1) - m(2,2)) * 2;
				mRotation = quat(
					0.25 * S,
					(m(0,1) + m(1,0)) / S,
					(m(0,2) + m(2,0)) / S,
					(m(2,1) - m(1,2)) / S
				);
			}
			else if (m(1,1) > m(2,2)) 
			{ 
				float S = sqrt(1.0 + m(1,1) - m(0,0) - m(2,2)) * 2;
				mRotation = quat(
					(m(0,1) + m(1,0)) / S,
					0.25 * S,
					(m(1,2) + m(2,1)) / S,
					(m(0,2) - m(2,0)) / S
				);
			}
			else 
			{ 
				float S = sqrt(1.0 + m(2,2) - m(0,0) - m(1,1)) * 2;
				mRotation = quat(
					(m(0,2) + m(2,0)) / S,
					(m(1,2) + m(2,1)) / S,
					0.25 * S,
					(m(1,0) - m(0,1)) / S
				);
			}

			mRotation = normalize(mRotation);
		}

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