#include "lightsource.h"

namespace tiny { namespace core { 

const core::vec3 LightSource::NO_ATTENUATION(0,0,1);
const core::vec3 LightSource::STANDARD_ATTENUATION(1,0,0);

LightSource::LightSource(const vec3 &color, const vec3 &position, const vec3 &direction)
	: mColor(color), mAttenuation(STANDARD_ATTENUATION), mCutoff(-1.0f), mCutoffExponent(0.0f)
{
	moveTo(position);
	alignWith(direction);
}

LightSource &LightSource::directional(const vec3 &color, const vec3 &direction)
{
	(*this) = LightSource(color, -normalize(direction) * 1.5e11f, direction);	
	mAttenuation = NO_ATTENUATION;	

	return (*this);
}

LightSource &LightSource::point(const vec3 &color, const vec3 &position)
{
	(*this) = LightSource(color, position);

	return (*this);
}

LightSource &LightSource::spot(const vec3 &color, const vec3 &position, const vec3 &direction, float cutoff, float cutoffExponent)
{
	(*this) = LightSource(color, position, direction);
	mCutoff = cutoff;
	mCutoffExponent = cutoffExponent;

	return (*this);
}




}}