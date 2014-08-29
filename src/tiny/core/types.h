#ifndef __TINY_CORE_TYPES_H__
#define __TINY_CORE_TYPES_H__

#include <tiny/math/matrix.h>
#include <tiny/math/quaternion.h>
#include <tiny/math/trigo.h>

namespace tiny { namespace core {
	
	using mat4 = tiny::math::matrix<float,4,4>;
	using vec2 = tiny::math::vector<float,2>;
	using vec3 = tiny::math::vector<float,3>;
	using vec4 = tiny::math::vector<float,4>;
	using quat = tiny::math::quaternion<float>;

	using ivec2 = tiny::math::vector<int, 2>;
	
	using rad  = tiny::math::radian;

}}

#endif