#ifndef __TINY_GL_TRANSFORMABLE_H__
#define __TINY_GL_TRANSFORMABLE_H__

#include <tiny/math/matrix.h>
#include <tiny/math/quaternion.h>
#include <tiny/math/trigo.h>

namespace tiny {

	namespace gl {


		class Transformable 
		{
			using rad  = tiny::math::radian;
			using vec3 = tiny::math::float3;
			using mat4 = tiny::math::float4x4;
			using quat = tiny::math::quaternion;
			
		public:
			Transformable() : mScale(1,1,1), mPosition(0,0,0), mRotation(0,0,0,1) {}

			Transformable &scale(const vec3 &factors)
			{
				scale(factors.x, factors.y, factors.z);
				return (*this);
			}

			Transformable &scale(float sx, float sy, float sz)
			{
				mScale.x *= sx;
				mScale.y *= sy;
				mScale.z *= sz;
				return (*this);
			}

			Transformable &rotate(const quat &rotation)
			{
				mRotation = (rotation * mRotation).normalized();
				return (*this);
			}

			Transformable &rotate(const vec3 axis, const rad &angle)
			{
				mRotation = (quat(axis, angle) * mRotation).normalized();
				return (*this);
			}

			Transformable &translate(const vec3 &displacement)
			{
				mPosition += displacement;
				return (*this);
			}

			Transformable &translate(float dx, float dy, float dz)
			{
				mPosition += vec3(dx, dy, dz);
				return (*this);
			}

			mat4 getMatrix() const
			{
				return getTranslationMatrix() * getRotationMatrix() * getScalingMatrix();
			}

			mat4 getTranslationMatrix() const
			{
				return mat4
				{
					1.0f, 0.0f, 0.0f, mPosition.x,
					0.0f, 1.0f, 0.0f, mPosition.y,
					0.0f, 0.0f, 1.0f, mPosition.z,
					0.0f, 0.0f, 0.0f, 1.0f,
				};
			}

			mat4 getScalingMatrix() const
			{
				return mat4
				{
					mScale.x, 0.0f,     0.0f,     0.0f,
					0.0f,     mScale.y, 0.0f,     0.0f,
					0.0f,     0.0f,     mScale.z, 0.0f,
					0.0f,     0.0f,     0.0f,     1.0f
				};
			}

			mat4 getRotationMatrix() const
			{
				return mRotation.getMatrix();
			}

		private:
			vec3 mScale;
			vec3 mPosition;
			quat mRotation;
		};

	}

}

#endif