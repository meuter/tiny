#ifndef __TINY_GL_TRANSFORMABLE_H__
#define __TINY_GL_TRANSFORMABLE_H__

#include <tiny/core/types.h>

namespace tiny { namespace core {

	class Transformable 
	{	
	public:
		Transformable();
		virtual ~Transformable();

		Transformable &scale(const vec3 &factors);
		Transformable &scale(float sx, float sy, float sz);

		Transformable &rotate(const quat &rotation);
		Transformable &rotate(const vec3 axis, const rad &angle);

		Transformable &translate(const vec3 &displacement);
		Transformable &translate(float dx, float dy, float dz);

		mat4 getMatrix() const;
		mat4 getTranslationMatrix() const;
		mat4 getScalingMatrix() const;
		mat4 getRotationMatrix() const;

	protected:
		vec3 mScale;
		vec3 mPosition;
		quat mRotation;
	};

}}

#endif