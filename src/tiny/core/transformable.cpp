#include "transformable.h"

namespace tiny { namespace core {

const vec3 Transformable::X_AXIS(1,0,0);
const vec3 Transformable::Y_AXIS(0,1,0);
const vec3 Transformable::Z_AXIS(0,0,1);


Transformable::Transformable() : mScaling(1,1,1), mPosition(0,0,0), mRotation(0,0,0,1), mParent(nullptr)
{
}


Transformable &Transformable::aimAt(const vec3 &target) 
{	
	vec3 newForward = normalize(target - position());
	vec3 newRight   = normalize(cross(up(), newForward));
	vec3 newUp      = normalize(cross(newForward, newRight));

	return rotateTo(quat(newForward, newUp));	
}

Transformable &Transformable::alignWith(const vec3 direction)
{
	Transformable t;

	t.moveTo(-direction);
	t.aimAt(0,0,0);

	return rotateTo(t.rotation());
}


mat4 Transformable::modelMatrix() const
{
	return parentMatrix() * translationMatrix() * rotationMatrix() * scalingMatrix();
}

mat4 Transformable::translationMatrix() const
{
	return mat4
	{
		1.0f, 0.0f, 0.0f, mPosition.x,
		0.0f, 1.0f, 0.0f, mPosition.y,
		0.0f, 0.0f, 1.0f, mPosition.z,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
}

mat4 Transformable::scalingMatrix() const
{
	return mat4
	{
		mScaling.x, 0.0f,       0.0f,       0.0f,
		0.0f,       mScaling.y, 0.0f,       0.0f,
		0.0f,       0.0f,       mScaling.z, 0.0f,
		0.0f,       0.0f,       0.0f,       1.0f
	};
}

mat4 Transformable::rotationMatrix() const
{
	auto l = left();
	auto u = up();
	auto f = forward();

	return mat4 {
		l.x,  u.x,  f.x,  0.0f,
		l.y,  u.y,  f.y,  0.0f,
		l.z,  u.z,  f.z,  0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

mat4 Transformable::parentMatrix() const
{
	static const mat4 identity {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,		
	};

	return mParent ? mParent->modelMatrix() : identity;
}

}}