#include "lightsource.h"

namespace tiny { namespace rendering { 


	LightSource LightSource::ambient(const vec3 &color, float intensity)
	{
		LightSource ambientLight;

		ambientLight.mColor = color;
		ambientLight.mIntensity = intensity;

		return ambientLight;
	}

	LightSource LightSource::directional(const vec3 &color, float intensity, const vec3 &direction)
	{
		LightSource directionalLight;

		directionalLight.moveTo(-normalize(direction) * 1.5e11f);
		directionalLight.alignWith(direction);

		directionalLight.mColor          = color;
		directionalLight.mAttenuation    = vec3(0,0,1);
		directionalLight.mIntensity      = intensity;
		directionalLight.mCutoff         = -1.0f;
		directionalLight.mCutoffExponent = 0.0f;

		return directionalLight;
	}

	LightSource LightSource::point(const vec3 &color, float intensity, const vec3 &position)
	{
		LightSource pointLight;

		pointLight.moveTo(position);

		pointLight.mColor          = color;
		pointLight.mIntensity      = intensity;
		pointLight.mAttenuation    = vec3(1,0,0);
		pointLight.mCutoff         = -1.0f;
		pointLight.mCutoffExponent = 0.0f;

		return pointLight;
	}

	LightSource LightSource::spot(const vec3 &color, float intensity, const vec3 &position, const vec3 &direction, float cutoff, float cutoffExponent)
	{
		LightSource spotLight;

		spotLight.moveTo(position);
		spotLight.alignWith(direction);

		spotLight.mColor          = color;
		spotLight.mIntensity      = intensity;
		spotLight.mAttenuation    = vec3(1,0,0);
		spotLight.mCutoff         = cutoff;
		spotLight.mCutoffExponent = cutoffExponent;

		return spotLight;
	}

}}