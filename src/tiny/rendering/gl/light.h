#ifndef __TINY_RENDERING_GL_DIRECTIONAL_H__
#define __TINY_RENDERING_GL_DIRECTIONAL_H__

#include <tiny/core/types.h>

namespace tiny { namespace rendering { namespace gl {

	class LightSource
	{
	public:	
		inline core::vec3 color()          const { return mColor; }
		inline core::vec3 direction()      const { return mDirection; }
		inline core::vec3 position()       const { return mPosition; }
		inline core::vec3 attenuation()    const { return mAttenuation; }	
		inline float      intensity()      const { return mIntensity; }
		inline float      cutoff()         const { return mCutoff; }
		inline float      cutoffExponent() const { return mCutoffExponent; }

		core::vec3 mColor;
		core::vec3 mDirection;
		core::vec3 mPosition;
		core::vec3 mAttenuation;	
		float mIntensity;		
		float mCutoff;
		float mCutoffExponent;
	};
}}}

#endif

