#ifndef __TINY_RENDERING_GL_LIGHTSOURCE_H__
#define __TINY_RENDERING_GL_LIGHTSOURCE_H__

#include <tiny/core/transformable.h>

namespace tiny { namespace rendering {

	class LightSource : public core::Transformable
	{		
		using vec3 = core::vec3;
		using quat = core::quat;
	public:	
		static LightSource directional(const vec3 &color, float intensity, const vec3 &direction);
		static LightSource point(const vec3 &color, float intensity, const vec3 &position);
		static LightSource spot(const vec3 &color, float intensity, const vec3 &position, const vec3 &direction, float cutoff, float cutoffExponent);
		static LightSource ambient(const vec3 &color, float intensity);

		LightSource() = default;

		inline core::vec3 color()          const { return mColor; }
		inline core::vec3 direction()      const { return forward(); }
		inline core::vec3 attenuation()    const { return mAttenuation; }	
		inline float      intensity()      const { return mIntensity; }
		inline float      cutoff()         const { return mCutoff; }
		inline float      cutoffExponent() const { return mCutoffExponent; }

	private:	
		core::vec3 mColor;
		core::vec3 mAttenuation;	
		float mIntensity;		
		float mCutoff;
		float mCutoffExponent;
	};
}}

#endif

