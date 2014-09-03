#include "transformable.h"

namespace tiny { namespace core {

Transformable::Transformable() : mScale(1,1,1), mPosition(0,0,0), mRotation(0,0,0,1) 
{

}

Transformable::~Transformable()
{

}


void Transformable::setScale(const vec3 &scale)
{
	mScale = scale;
}

void Transformable::setScale(float sx, float sy, float sz)
{
	mScale = vec3(sx, sy, sz);
}

void Transformable::setRotation(const quat &rotation)
{
	mRotation = rotation;
}

void Transformable::setRotation(const vec3 axis, const rad &angle)
{
	mRotation = quat(axis, angle);
}

void Transformable::setPosition(float dx, float dy, float dz)
{
	mPosition = vec3(dx, dy, dz);
}

void Transformable::setPosition(const vec3 &displacement)
{
	mPosition = displacement;
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