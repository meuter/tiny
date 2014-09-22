#ifndef __TINY_RENDERING_GL_DIRECTIONAL_H__
#define __TINY_RENDERING_GL_DIRECTIONAL_H__

#include <tiny/core/types.h>

namespace tiny { namespace rendering { namespace gl {

	class AmbientLight
	{
	public:	
		AmbientLight() : mColor(1,1,1), mIntensity(0.2f) {}
		AmbientLight(const core::vec3 &color, const float &intensity) : mColor(color), mIntensity(intensity) {}

		inline core::vec3 color()     const { return mColor; }
		inline float      intensity() const { return mIntensity; }

	private:
		core::vec3 mColor;
		float mIntensity;		
	};

	class DirectionalLight : public AmbientLight
	{
	public:	
		DirectionalLight() : mDirection(0,0,0) {}
		DirectionalLight(const core::vec3 &color, const float &intensity, const core::vec3 &direction) 
			: AmbientLight(color, intensity), mDirection(direction) {}

		inline core::vec3 direction() const { return mDirection; }

	private:		
		core::vec3 mDirection;
	};


	class PointLight : public AmbientLight
	{
	public:	
		PointLight() : mPosition(0,0,0), mAttenuation(1,0,0) 
		{
			computeRange();
		}

		PointLight(const core::vec3 &color, float intensity, const core::vec3 &position, const core::vec3 attenuation = core::vec3(1,0,0))
			: AmbientLight(color, intensity), mPosition(position), mAttenuation(attenuation)
		{
			computeRange();
		}

		void computeRange()
		{			
			float a = mAttenuation.x;
			float b = mAttenuation.y;
			float c = mAttenuation.z - 256 * intensity() * max(color());

			float delta = b*b - 4*a-c;

			mRange  = (-b + sqrt(delta)) / 2*a;
		}

		inline core::vec3 position()    const { return mPosition; }
		inline core::vec3 attenuation() const { return mAttenuation; }	
		inline float      range()       const { return mRange; }

	private:
		core::vec3 mPosition;
		core::vec3 mAttenuation;	
		float mRange;
	};

	class SpotLight : public PointLight
	{
	public:	
		SpotLight() : mDirection(0,0,0), mCutoff(0.6f) {}
		SpotLight(const core::vec3 &color, float intensity, const core::vec3 &position, const core::vec3 &direction, const core::vec3 attenuation = core::vec3(1,0,0), float cutoff=0.6f)
			: PointLight(color, intensity, position, attenuation), mDirection(direction), mCutoff(cutoff) {}

		inline core::vec3 direction() const { return mDirection; }
		inline float      cutoff()    const { return mCutoff; }
	private:
		core::vec3 mDirection;
		float mCutoff;

	};

}}}

#endif

