#ifndef __TINY_CORE_TYPES_H__
#define __TINY_CORE_TYPES_H__

#include <tiny/math/matrix.h>
#include <tiny/math/quaternion.h>
#include <tiny/math/trigo.h>
#include <chrono>

namespace tiny { namespace core {
	
	using mat2   = tiny::math::matrix<float,2,2>;
	using mat2x2 = tiny::math::matrix<float,2,2>;
	using mat2x3 = tiny::math::matrix<float,2,3>;
	using mat2x4 = tiny::math::matrix<float,2,4>;

	using mat3   = tiny::math::matrix<float,3,3>;
	using mat3x2 = tiny::math::matrix<float,3,2>;
	using mat3x3 = tiny::math::matrix<float,3,3>;
	using mat3x4 = tiny::math::matrix<float,3,4>;

	using mat4   = tiny::math::matrix<float,4,4>;
	using mat4x2 = tiny::math::matrix<float,4,2>;
	using mat4x3 = tiny::math::matrix<float,4,3>;
	using mat4x4 = tiny::math::matrix<float,4,4>;

	using vec2 = tiny::math::vector<float,2>;
	using vec3 = tiny::math::vector<float,3>;
	using vec4 = tiny::math::vector<float,4>;

	using ivec2 = tiny::math::vector<int, 2>;
	using ivec3 = tiny::math::vector<int, 3>;
	using ivec4 = tiny::math::vector<int, 4>;

	using bvec2 = tiny::math::vector<bool, 2>;
	using bvec3 = tiny::math::vector<bool, 3>;
	using bvec4 = tiny::math::vector<bool, 4>;

	using quat = tiny::math::quaternion<float>;

	using rad = tiny::math::radian;

	using sec = std::chrono::duration<double>;
}}

#endif