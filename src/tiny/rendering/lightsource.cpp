#include "lightsource.h"

namespace tiny { namespace rendering { 

	LightSource LightSource::directional(const vec3 &color, float intensity, const vec3 &direction)
	{
		LightSource directionalLight;

		directionalLight.mColor          = color;
		directionalLight.mDirection      = normalize(direction);
		directionalLight.mPosition       = -normalize(direction) * 1.5e11f;
		directionalLight.mAttenuation    = vec3(0,0,1);
		directionalLight.mIntensity      = intensity;
		directionalLight.mCutoff         = -1.0f;
		directionalLight.mCutoffExponent = 0.0f;

		return directionalLight;
	}

	LightSource LightSource::point(const vec3 &color, float intensity, const vec3 &position)
	{
		LightSource pointLight;

		pointLight.mColor          = color;
		pointLight.mIntensity      = intensity;
		pointLight.mPosition       = position;
		pointLight.mDirection      = vec3(0,0,0);
		pointLight.mAttenuation    = vec3(1,0,0);
		pointLight.mCutoff         = -1.0f;
		pointLight.mCutoffExponent = 0.0f;

		return pointLight;
	}

	LightSource LightSource::spot(const vec3 &color, float intensity, const vec3 &position, const vec3 &direction, float cutoff, float cutoffExponent)
	{
		LightSource spotLight;

		spotLight.mColor          = color;
		spotLight.mIntensity      = intensity;
		spotLight.mPosition       = position;
		spotLight.mDirection      = normalize(direction);
		spotLight.mAttenuation    = vec3(1,0,0);
		spotLight.mCutoff         = cutoff;
		spotLight.mCutoffExponent = cutoffExponent;

		return spotLight;
	}

	LightSource LightSource::ambient(const vec3 &color, float intensity)
	{
		LightSource ambientLight;

		ambientLight.mColor = color;
		ambientLight.mDirection = vec3(0,0,0);
		ambientLight.mPosition = vec3(0,0,0);
		ambientLight.mAttenuation = vec3(0,0,1);
		ambientLight.mIntensity = intensity;
		ambientLight.mCutoff = -1.0f;
		ambientLight.mCutoffExponent = 0.0f;

		return ambientLight;
	}

}}