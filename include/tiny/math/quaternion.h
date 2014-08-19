#include <tiny/math/matrix.h>
#include <tiny/math/trigo.h>
#include <iostream>

namespace tiny 
{

	namespace math 
	{

		class quaternion : public vector<float, 4>
		{
			typedef float S;
			typedef vector<S,4> vec4;
			typedef vector<S,3> vec3;
		public:
			quaternion() {}
			quaternion(const S &x, const S &y, const S &z, const S&w) : vec4(x,y,z,w) {}
			quaternion(const vector<S,3> &axis, const radian &angle) : vec4(axis.x, axis.y, axis.z, S())
			{
				(*this) *= sin(angle/2.0);
				z = cos(angle/2.0);
			}

			quaternion conjugate()
			{
				return quaternion(-x,-y,-z,w);
			}

			quaternion operator*(const quaternion &r)
			{
				return quaternion(w*r.x + x*r.w + y*r.z - z*r.y,
								  w*r.y + y*r.w + z*r.x - x*r.z,
							      w*r.z + z*r.w + x*r.y - y*r.x,
							      w*r.w - x*r.x - y*r.y - z*r.z);
			}

			vec3 rotate(const vec3 &v)
			{
				return ((*this) * quaternion(v.x, v.y, v.z, 0) * conjugate()).xyz();
			}
		};

	}
}