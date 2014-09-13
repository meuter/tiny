#ifndef __TINY_GL_TRANSFORMABLE_H__
#define __TINY_GL_TRANSFORMABLE_H__

#include <tiny/core/types.h>

namespace tiny { namespace core {

	class Transformable 
	{	
	public:
		static const vec3 X_AXIS, Y_AXIS, Z_AXIS;

		Transformable();
		virtual ~Transformable();

		inline void move(const vec3 &direction, float amount)    { mPosition += amount * direction; }
		inline void move(float dx, float dy, float dz)           { mPosition += vec3(dx, dy, dz); }
		inline void moveTo(const vec3 &position)                 { mPosition = position; }
		inline void moveTo(float x, float y, float z)            { mPosition = vec3(x,y,z); }

		inline void rotate(const vec3 axis, const rad &angle)    { mRotation = normalize(quat(axis, angle) * mRotation); }
		inline void rotateTo(const vec3 axis, const rad &angle)  { mRotation = quat(axis, angle); }

		inline void roll(rad angle)                              { rotate(forward(), angle); }
		inline void yaw(rad angle)                               { rotate(up(), angle); }
		inline void pitch(rad angle)                             { rotate(right(), angle); }

		inline void scale(const vec3 &factors)                   { scale(factors.x, factors.y, factors.z); }
		inline void scale(float fx, float fy, float fz)          { mScale.x *= fx; mScale.y *= fy; mScale.z *= fz; }
		inline void scaleTo(const vec3 &scale)                   { mScale = scale; }
		inline void scaleTo(float sx, float sy, float sz)        { mScale = vec3(sx, sy, sz); }

		inline vec3 right()    const { return mRotation.rotate(X_AXIS); }
		inline vec3 left()     const { return mRotation.rotate(-X_AXIS); }
		inline vec3 up()       const { return mRotation.rotate(Y_AXIS); }
		inline vec3 down()     const { return mRotation.rotate(-Y_AXIS); }
		inline vec3 forward()  const { return mRotation.rotate(Z_AXIS); }
		inline vec3 backward() const { return mRotation.rotate(-Z_AXIS); }

		mat4 getMatrix()            const;
		mat4 getTranslationMatrix() const;
		mat4 getScalingMatrix()     const;
		mat4 getRotationMatrix()    const;

	protected:
		vec3 mScale;
		vec3 mPosition;
		quat mRotation;
	};

}}

#endif