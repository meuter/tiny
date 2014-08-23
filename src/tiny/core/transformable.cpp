#include "transformable.h"

namespace tiny { namespace core {

Transformable::Transformable() : mScale(1,1,1), mPosition(0,0,0), mRotation(0,0,0,1) 
{

}

Transformable::~Transformable()
{

}


Transformable &Transformable::scale(const vec3 &factors)
{
	scale(factors.x, factors.y, factors.z);
	return (*this);
}

Transformable &Transformable::scale(float sx, float sy, float sz)
{
	mScale.x *= sx;
	mScale.y *= sy;
	mScale.z *= sz;
	return (*this);
}

Transformable &Transformable::rotate(const quat &rotation)
{
	mRotation = (rotation * mRotation).normalized();
	return (*this);
}

Transformable &Transformable::rotate(const vec3 axis, const rad &angle)
{
	mRotation = (quat(axis, angle) * mRotation).normalized();
	return (*this);
}

Transformable &Transformable::translate(const vec3 &displacement)
{
	mPosition += displacement;
	return (*this);
}

Transformable &Transformable::translate(float dx, float dy, float dz)
{
	mPosition += vec3(dx, dy, dz);
	return (*this);
}

mat4 Transformable::getMatrix() const
{
	return getTranslationMatrix() * getRotationMatrix() * getScalingMatrix();
}

mat4 Transformable::getTranslationMatrix() const
{
	return mat4
	{
		1.0f, 0.0f, 0.0f, mPosition.x,
		0.0f, 1.0f, 0.0f, mPosition.y,
		0.0f, 0.0f, 1.0f, mPosition.z,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
}

mat4 Transformable::getScalingMatrix() const
{
	return mat4
	{
		mScale.x, 0.0f,     0.0f,     0.0f,
		0.0f,     mScale.y, 0.0f,     0.0f,
		0.0f,     0.0f,     mScale.z, 0.0f,
		0.0f,     0.0f,     0.0f,     1.0f
	};
}

mat4 Transformable::getRotationMatrix() const
{
	return mRotation.getMatrix();
}


}}