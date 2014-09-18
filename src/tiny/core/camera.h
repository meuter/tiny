#ifndef __TINY_CORE_CAMERA_H__
#define __TINY_CORE_CAMERA_H__

#include "types.h"
#include "transformable.h"
#include <tiny/math/trigo.h>
#include <algorithm>

namespace tiny { namespace core {

	class Camera : public Transformable
	{
	public:	
		static Camera withPerspective(rad fieldOfView, float aspectRatio, float zNear, float zFar);

		Camera() = default;
		Camera(const mat4 &projection) : mProjection(projection) {}

		virtual ~Camera() = default;

		void lookAt(float x, float y, float z);
		void lookAt(const vec3 target);

		const mat4 &projectionMatrix() const;
		mat4 viewMatrix()              const;

	private:
		mat4 mProjection;
	};

}}

#endif