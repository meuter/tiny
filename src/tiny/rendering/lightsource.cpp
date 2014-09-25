#include "lightsource.h"

namespace tiny { namespace rendering { 

const core::vec3 LightSource::NO_ATTENUATION(0,0,1);
const core::vec3 LightSource::STANDARD_ATTENUATION(1,0,0);

LightSource LightSource::directional(const vec3 &color, const vec3 &direction)
{
	return LightSource(color, -normalize(direction) * 1.5e11f, direction).setAttenuation(NO_ATTENUATION);
}

LightSource LightSource::point(const vec3 &color, const vec3 &position)
{
	return LightSource(color, position);
}

LightSource LightSource::spot(const vec3 &color, const vec3 &position, const vec3 &direction, float cutoff, float cutoffExponent)
{
	return LightSource(color, position, direction).setCutoff(cutoff, cutoffExponent);
}

LightSource::LightSource(const vec3 &color, const vec3 &position, const vec3 &direction)
	: mColor(color), mAttenuation(STANDARD_ATTENUATION), mCutoff(-1.0f), mCutoffExponent(0.0f)
{
	moveTo(position);
	alignWith(direction);
}

LightSource &LightSource::setAttenuation(const vec3 &attenuation)
{
	mAttenuation = attenuation;
	return (*this);
}

LightSource &LightSource::setCutoff(float factor, float exponent)
{
	mCutoff = factor;
	mCutoffExponent = exponent;
	return (*this);
}

}}