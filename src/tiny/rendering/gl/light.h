#ifndef __TINY_RENDERING_GL_DIRECTIONAL_H__
#define __TINY_RENDERING_GL_DIRECTIONAL_H__

#include <tiny/core/types.h>

namespace tiny { namespace rendering { namespace gl {

	class DirectionalLight
	{
	public:	
		DirectionalLight() : mColor(1,1,1), mIntensity(0), mDirection(0,0,0) {}
		DirectionalLight(const core::vec3 &color, const float &intensity, const core::vec3 &direction) 
			: mColor(color), mIntensity(intensity), mDirection(direction) {}

		inline core::vec3 color()     const { return mColor; }
		inline core::vec3 direction() const { return mDirection; }
		inline float      intensity() const { return mIntensity; }
	private:		
		core::vec3 mColor;
		float mIntensity;
		core::vec3 mDirection;
	};


	class PointLight
	{
	public:	
		PointLight() : mColor(1,1,1), mIntensity(0), mPosition(0,0,0), mAttenuation(1,0,0) 
		{
			computeRange();
		}

		PointLight(const core::vec3 &color, float intensity, const core::vec3 &position, const core::vec3 attenuation = core::vec3(1,0,0))
			: mColor(color), mIntensity(intensity), mPosition(position), mAttenuation(attenuation)
		{
			computeRange();
		}

		void computeRange()
		{			
			float a = mAttenuation.x;
			float b = mAttenuation.y;
			float c = mAttenuation.z - 256 * mIntensity * max(mColor);

			float delta = b*b - 4*a-c;

			mRange  = (-b + sqrt(delta)) / 2*a;
		}

		inline core::vec3 color()       const { return mColor; }
		inline core::vec3 position()    const { return mPosition; }
		inline core::vec3 attenuation() const { return mAttenuation; }	
		inline float      range()       const { return mRange; }
		inline float      intensity()   const { return mIntensity; }

		core::vec3 mColor;
		float mIntensity;
		core::vec3 mPosition;
		core::vec3 mAttenuation;	
	private:
		float mRange;
	};

}}}

#endif

