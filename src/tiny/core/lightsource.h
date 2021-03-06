#ifndef __TINY_RENDERING_GL_LIGHTSOURCE_H__
#define __TINY_RENDERING_GL_LIGHTSOURCE_H__

#include "transformable.h"

namespace tiny { namespace core {

	class LightSource : public Transformable
	{		
		using vec3 = core::vec3;
		using quat = core::quat;
	public:	

		LightSource() = default;
		LightSource(const vec3 &color, const vec3 &position = vec3(0,0,0), const vec3 &direction = vec3(1,1,1));

		inline vec3  color()          const { return mColor; }
		inline vec3  attenuation()    const { return mAttenuation; }	
		inline float cutoff()         const { return mCutoff; }
		inline float cutoffExponent() const { return mCutoffExponent; }

		LightSource &directional(const vec3 &color, const vec3 &direction);
		LightSource &point(const vec3 &color, const vec3 &position);
		LightSource &spot(const vec3 &color, const vec3 &position, const vec3 &direction, float cutoff, float cutoffExponent);

	private:	
		core::vec3 mColor;
		core::vec3 mAttenuation;	
		float mCutoff;
		float mCutoffExponent;
	};
}}

#endif

