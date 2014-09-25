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

		inline void move(const vec3 &direction, float amount)    { mPosition += amount * direction; } 
		inline void moveTo(const vec3 &position)                 { mPosition = position; } 
		inline void moveTo(float x, float y, float z)            { mPosition = vec3(x,y,z); } 

		inline void rotate(const vec3 axis, const rad &angle)    { rotate(quat(axis, angle)); } 
		inline void rotate(const quat &rotation)                 { mRotation = normalize(rotation * mRotation); } 
		inline void rotateTo(const vec3 axis, const rad &angle)  { rotateTo(quat(axis, angle)); } 
		inline void rotateTo(const quat &rotation)               { mRotation = normalize(rotation); } 

		inline void scale(float factor)                          { scale(factor, factor, factor); } 
		inline void scale(const vec3 &factors)                   { scale(factors.x, factors.y, factors.z); } 
		inline void scale(float fx, float fy, float fz)          { mScaling.x *= fx; mScaling.y *= fy; mScaling.z *= fz; } 
		inline void scaleTo(const vec3 &scale)                   { mScaling = scale; } 
		inline void scaleTo(float sx, float sy, float sz)        { mScaling = vec3(sx, sy, sz); } 


		void aimAt(float x, float y, float z)
		{
			aimAt(vec3(x,y,z));
		}

		void aimAt(const vec3 &target) 
		{	
			vec3 newForward = normalize(target - position());
			vec3 newRight   = normalize(cross(up(), newForward));
			vec3 newUp      = normalize(cross(newForward, newRight));

			rotateTo(quat(newForward, newUp));	
		}

		void alignWith(const vec3 direction)
		{
			Transformable t;

			t.moveTo(-direction);
			t.aimAt(0,0,0);

			rotateTo(t.rotation());
		}


		inline vec3 left()            const                      { return mRotation.rotate( X_AXIS); }
		inline vec3 right()           const                      { return mRotation.rotate(-X_AXIS); }
		inline vec3 up()              const                      { return mRotation.rotate( Y_AXIS); }
		inline vec3 down()            const                      { return mRotation.rotate(-Y_AXIS); }
		inline vec3 forward()         const                      { return mRotation.rotate( Z_AXIS); }
		inline vec3 backward()        const                      { return mRotation.rotate(-Z_AXIS); }

		inline vec3 position()        const                      { return mPosition; }
		inline vec3 scaling()         const                      { return mScaling; }
		inline quat rotation()        const                      { return mRotation; }

		mat4 modelMatrix()            const;
		mat4 translationMatrix()      const;
		mat4 scalingMatrix()          const;
		mat4 rotationMatrix()         const;

	private:
		vec3 mScaling;
		vec3 mPosition;
		quat mRotation;
	};

}}

#endif