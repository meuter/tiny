#ifndef __TINY_CORE_CAMERA_H__
#define __TINY_CORE_CAMERA_H__

#include <tiny/math/trigo.h>

#include "types.h"
#include "transformable.h"
#include "window.h"
#include "inputs.h"

namespace tiny { namespace core {

	class Camera : public Transformable
	{
	public:	
		static Camera withPerspective(rad fieldOfView, float aspectRatio, float zNear, float zFar);

		Camera() = default;
		Camera(const mat4 &projection) : mProjection(projection) {}

		virtual ~Camera() = default;

		void update(Window &window, Inputs &inputs, sec dt);

		const mat4 &projectionMatrix() const;
		mat4 viewMatrix()              const;

	private:
		mat4 mProjection;
	};

}}

#endif