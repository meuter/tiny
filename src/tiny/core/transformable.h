#ifndef __TINY_GL_TRANSFORMABLE_H__
#define __TINY_GL_TRANSFORMABLE_H__

#include <tiny/core/types.h>

namespace tiny { namespace core {

	class Transformable 
	{	
	public:
		static const vec3 X_AXIS, Y_AXIS, Z_AXIS;

		Transformable();
		Transformable(const Transformable &other) = default;
		Transformable(Transformable &&other) = default;

		virtual ~Transformable() = default;

		Transformable &operator=(const Transformable &other) = default;
		Transformable &operator=(Transformable &&other) = default;

		inline vec3 left()            const                                { return mRotation.rotate( X_AXIS); }
		inline vec3 right()           const                                { return mRotation.rotate(-X_AXIS); }
		inline vec3 up()              const                                { return mRotation.rotate( Y_AXIS); }
		inline vec3 down()            const                                { return mRotation.rotate(-Y_AXIS); }
		inline vec3 forward()         const                                { return mRotation.rotate( Z_AXIS); }
		inline vec3 backward()        const                                { return mRotation.rotate(-Z_AXIS); }

		inline vec3 position()        const                                { return mPosition; }
		inline vec3 scaling()         const                                { return mScaling; }
		inline quat rotation()        const                                { return mRotation; }

		inline Transformable &move(const vec3 &direction, float amount)    { mPosition += amount * direction; return (*this); } 
		inline Transformable &moveTo(const vec3 &position)                 { mPosition = position; return (*this); } 
		inline Transformable &moveTo(float x, float y, float z)            { mPosition = vec3(x,y,z); return (*this); } 

		inline Transformable &rotate(const vec3 axis, const rad &angle)    { return rotate(quat(axis, angle));  } 
		inline Transformable &rotate(const quat &rotation)                 { mRotation = normalize(rotation * mRotation); return (*this); } 
		inline Transformable &rotateTo(const vec3 axis, const rad &angle)  { return rotateTo(quat(axis, angle)); } 
		inline Transformable &rotateTo(const quat &rotation)               { mRotation = normalize(rotation); return (*this); } 

		inline Transformable &scale(float factor)                          { return scale(factor, factor, factor); } 
		inline Transformable &scale(const vec3 &factors)                   { return scale(factors.x, factors.y, factors.z); } 
		inline Transformable &scale(float fx, float fy, float fz)          { mScaling.x *= fx; mScaling.y *= fy; mScaling.z *= fz; return (*this); } 
		inline Transformable &scaleTo(const vec3 &scale)                   { mScaling = scale; return (*this); } 
		inline Transformable &scaleTo(float sx, float sy, float sz)        { mScaling = vec3(sx, sy, sz); return (*this); } 

		inline Transformable &attachTo(const Transformable &parent)        { mParent = &parent; return *this; }
		inline Transformable &detach()                                     { mParent = nullptr; return *this; }
		inline Transformable &aimAt(float x, float y, float z)             { return aimAt(vec3(x,y,z)); }

		Transformable &aimAt(const vec3 &target);
		Transformable &alignWith(const vec3 direction);

		mat4 modelMatrix()            const;
		mat4 translationMatrix()      const;
		mat4 scalingMatrix()          const;
		mat4 rotationMatrix()         const;
		mat4 parentMatrix()           const;

	private:
		vec3 mScaling;
		vec3 mPosition;
		quat mRotation;
		const Transformable *mParent;
	};

}}

#endif