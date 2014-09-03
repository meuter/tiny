#ifndef __TINY_GL_TRANSFORMABLE_H__
#define __TINY_GL_TRANSFORMABLE_H__

#include <tiny/core/types.h>

namespace tiny { namespace core {

	class Transformable 
	{	
	public:
		Transformable();
		virtual ~Transformable();

		void setPosition(float dx, float dy, float dz);
		void setPosition(const vec3 &displacement);

		void setScale(const vec3 &factors);
		void setScale(float sx, float sy, float sz);

		void setRotation(const quat &rotation);
		void setRotation(const vec3 axis, const rad &angle);

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