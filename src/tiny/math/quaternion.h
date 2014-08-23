#ifndef __TINY_MATH_QUATERNION_H__
#define __TINY_MATH_QUATERNION_H__

#include "matrix.h"
#include "trigo.h"
#include <iostream>

namespace tiny 
{
	namespace math 
	{

		template<typename S>
		struct quaternion : vector<S,4>
		{
			using vector<S,4>::x;
			using vector<S,4>::y;
			using vector<S,4>::z;
			using vector<S,4>::w;

			using vec4 = vector<S,4>;
			using vec3 = vector<S,3>;
			using mat4 = matrix<S,4,4>;
			using quat = quaternion<S>;

			quaternion() {}
			quaternion(const vec4 &v) : vec4(v) {}
			quaternion(const S &x, const S &y, const S &z, const S &w) : vec4(x,y,z,w) {}
			quaternion(const vec3 &axis, const radian &angle) : vec4(axis) 
			{				
				(*this) *= sin(angle/2.0);
				w = cos(angle/2.0);
			}

			quat conjugate() const
			{
				return quat(-x,-y,-z,w);
			}

			quat operator*(const quat &r) const 
			{
				return quat(w*r.x + x*r.w + y*r.z - z*r.y,
								  w*r.y + y*r.w + z*r.x - x*r.z,
							      w*r.z + z*r.w + x*r.y - y*r.x,
							      w*r.w - x*r.x - y*r.y - z*r.z);
			}

			vec3 rotate(const vec3 &v) const
			{
				return ((*this) * quat(v.x, v.y, v.z, 0) * conjugate()).xyz();
			}

			mat4 getMatrix() const 
			{
				return mat4 {
					1.0f - 2.0f * (y*y + z*z), 2.0f * (x*y - w*z),        2.0f * (x*z + w*y),        0.0f,
					2.0f * (x*y + w*z),        1.0f - 2.0f * (x*x + z*z), 2.0f * (y*z - w*x),        0.0f,
					2.0f * (x*z - w*y),        2.0f * (y*z + w*x),        1.0f - 2.0f * (x*x + y*y), 0.0f,
					0.0f,                      0.0f,                      0.0f,                      1.0f,
				};
			}
		};

	}
}

#endif