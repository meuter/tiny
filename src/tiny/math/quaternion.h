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
			float t = trace(m);
			
			if(t > 0)
			{
				float s = 0.5f / sqrtf(t + 1.0f);
				w = 0.25f / s;
				x = (m(1,2) - m(2,1)) * s;
				y = (m(2,0) - m(0,2)) * s;
				z = (m(0,1) - m(1,0)) * s;
			}
			else if(m(0,0) > m(1,1) && m(0,0) > m(2,2))
			{
				float s = 2.0f * sqrtf(1.0f + m(0,0) - m(1,1) - m(2,2));
				w = (m(1,2) - m(2,1)) / s;
				x = 0.25f * s;
				y = (m(1,0) + m(0,1)) / s;
				z = (m(2,0) + m(0,2)) / s;
			}
			else if(m(1,1) > m(2,2))
			{
				float s = 2.0f * sqrtf(1.0f + m(1,1) - m(0,0) - m(2,2));
				w = (m(2,0) - m(0,2)) / s;
				x = (m(1,0) + m(0,1)) / s;
				y = 0.25f * s;
				z = (m(2,1) + m(1,2)) / s;
			}
			else
			{
				float s = 2.0f * sqrtf(1.0f + m(2,2) - m(1,1) - m(0,0));
				w = (m(0,1) - m(1,0)) / s;
				x = (m(2,0) + m(0,2)) / s;
				y = (m(1,2) + m(2,1)) / s;
				z = 0.25f * s;
			}
			
			(*this) = normalize(*this);
		}

		quaternion(const vec3 &forward, const vec3 &up)
		{		
			vec3 left = normalize(cross(up, forward));

			mat3 m =
			{
				left.x,    left.y,    left.z, 
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