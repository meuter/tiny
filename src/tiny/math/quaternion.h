#ifndef __TINY_MATH_QUATERNION_H__
#define __TINY_MATH_QUATERNION_H__

#include "matrix.h"
#include "trigo.h"

namespace tiny { namespace math {

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
		using mat3 = matrix<S,3,3>;
		using quat = quaternion<S>;

		quaternion() {}
		quaternion(const vec4 &v) : vec4(v) {}
		quaternion(const S &x, const S &y, const S &z, const S &w) : vec4(x,y,z,w) {}
		quaternion(const vec3 &axis, const radian &angle) : vec4(axis) 
		{				
			(*this) *= sin(angle/2.0);
			w = cos(angle/2.0);
		}
		quaternion(const mat3 &m)
		{
			w = std::sqrt(std::max(0.0f, 1.0f+m(0,0)+m(1,1)+m(2,2))) / 2;
			x = std::sqrt(std::max(0.0f, 1.0f+m(0,0)-m(1,1)-m(2,2))) / 2;
			y = std::sqrt(std::max(0.0f, 1.0f-m(0,0)+m(1,1)-m(2,2))) / 2;
			z = std::sqrt(std::max(0.0f, 1.0f-m(0,0)-m(1,1)+m(2,2))) / 2;

			x = std::copysign(x, m(2,1)-m(1,2));
			y = std::copysign(y, m(0,2)-m(2,0));
			z = std::copysign(z, m(1,0)-m(0,1));
		}
		quaternion(const vec3 &forward, const vec3 &up)
		{
			vec3 right = normalize(cross(up, forward));

			mat3 m =
			{
				right.x,   right.y,   right.z,
				up.x,      up.y,      up.z,
				forward.x, forward.y, forward.z,
			};

			(*this) = quaternion(m);
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
	};

}}

#endif