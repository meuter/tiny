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
			typedef matrix<S,4,4> mat4;
		public:
			quaternion() {}
			quaternion(const S &x, const S &y, const S &z, const S&w) : vec4(x,y,z,w) {}
			quaternion(const vec3 &axis, const radian &angle) : vec4(axis) 
			{				
				(*this) *= (float)sin(angle/2.0);
				w = cos(angle/2.0);
			}

			quaternion conjugate() const
			{
				return quaternion(-x,-y,-z,w);
			}

			quaternion operator*(const quaternion &r) const 
			{
				return quaternion(w*r.x + x*r.w + y*r.z - z*r.y,
								  w*r.y + y*r.w + z*r.x - x*r.z,
							      w*r.z + z*r.w + x*r.y - y*r.x,
							      w*r.w - x*r.x - y*r.y - z*r.z);
			}

			quaternion &operator *=(const S &f)
			{
				vec4::operator*=(f);
				return (*this);
			}

			quaternion &operator *=(const quaternion &r)
			{
				quaternion before = (*this);
				(*this) = before * r;
				return (*this);
			}

			vec3 rotate(const vec3 &v) const
			{
				return ((*this) * quaternion(v.x, v.y, v.z, 0) * conjugate()).xyz();
			}

			mat4 getMatrix() const 
			{
				return mat4{
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f,
				};
			}
		};

	}
}