#include "transformable.h"

namespace tiny { namespace core {

const vec3 Transformable::X_AXIS(1,0,0);
const vec3 Transformable::Y_AXIS(0,1,0);
const vec3 Transformable::Z_AXIS(0,0,1);


Transformable::Transformable() : mScaling(1,1,1), mPosition(0,0,0), mRotation(0,0,0,1) 
{

}

Transformable::~Transformable()
{

}

mat4 Transformable::getModel() const
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
		mScaling.x, 0.0f,       0.0f,       0.0f,
		0.0f,       mScaling.y, 0.0f,       0.0f,
		0.0f,       0.0f,       mScaling.z, 0.0f,
		0.0f,       0.0f,       0.0f,       1.0f
	};
}

mat4 Transformable::getRotationMatrix() const
{
	auto r = right(); 
	auto u = up();
	auto f = forward();

	return mat4 
	{
		r.x,  r.y,  r.z,  0.0f,
		u.x,  u.y,  u.z,  0.0f,
		f.x,  f.y,  f.z,  0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}


}}