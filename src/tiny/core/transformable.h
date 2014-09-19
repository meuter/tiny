#ifndef __TINY_GL_TRANSFORMABLE_H__
#define __TINY_GL_TRANSFORMABLE_H__

#include <tiny/core/types.h>

namespace tiny { namespace core {

	class Transformable 
	{	
	public:
		static const vec3 X_AXIS, Y_AXIS, Z_AXIS;

		Transformable();
		virtual ~Transformable() = default;

		inline Transformable &move(const vec3 &direction, float amount)    { mPosition += amount * direction; return (*this); } 
		inline Transformable &moveTo(const vec3 &position)                 { mPosition = position; return (*this); } 
		inline Transformable &moveTo(float x, float y, float z)            { mPosition = vec3(x,y,z); return (*this); } 

		inline Transformable &rotate(const vec3 axis, const rad &angle)    { rotate(quat(axis, angle)); return (*this); } 
		inline Transformable &rotate(const quat &rotation)                 { mRotation = normalize(rotation * mRotation); return (*this); } 
		inline Transformable &rotateTo(const vec3 axis, const rad &angle)  { rotateTo(quat(axis, angle)); return (*this); } 
		inline Transformable &rotateTo(const quat &rotation)               { mRotation = normalize(rotation); return (*this); } 

		inline Transformable &scale(float factor)                          { scale(factor, factor, factor); return (*this); } 
		inline Transformable &scale(const vec3 &factors)                   { scale(factors.x, factors.y, factors.z); return (*this); } 
		inline Transformable &scale(float fx, float fy, float fz)          { mScaling.x *= fx; mScaling.y *= fy; mScaling.z *= fz; return (*this); } 
		inline Transformable &scaleTo(const vec3 &scale)                   { mScaling = scale; return (*this); } 
		inline Transformable &scaleTo(float sx, float sy, float sz)        { mScaling = vec3(sx, sy, sz); return (*this); } 

		inline vec3 left()            const                      { return mRotation.rotate( X_AXIS); }
		inline vec3 right()           const                      { return mRotation.rotate(-X_AXIS); }
		inline vec3 up()              const                      { return mRotation.rotate( Y_AXIS); }
		inline vec3 down()            const                      { return mRotation.rotate(-Y_AXIS); }
		inline vec3 forward()         const                      { return mRotation.rotate( Z_AXIS); }
		inline vec3 backward()        const                      { return mRotation.rotate(-Z_AXIS); }

		inline const vec3 &position() const                      { return mPosition; }
		inline const vec3 &scaling()  const                      { return mScaling; }
		inline const quat &rotation() const                      { return mRotation; }

		mat4 modelMatrix()            const;
		mat4 translationMatrix()      const;
		mat4 scalingMatrix()          const;
		mat4 rotationMatrix()         const;

	protected:
		vec3 mScaling;
		vec3 mPosition;
		quat mRotation;
	};

}}

#endif