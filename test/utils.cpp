#include "utils.h"
#include <cassert>

namespace core
{
	/// solve quadratic equation
	bool    SolveQuadratic( const real& a, const real& b, const real&c, real& x1, real& x2 )
	{
		real d = b*b - 4*a*c;
		if ( d < 0 )
			return false;
		else
		{
			real den = 1/(2*a);
			x1 = (-b - sqrt(d))*den;
			x2 = (-b + sqrt(d))*den;
			return true;
		}
	}

	matrix4x4 translation_matrix(vector3 const& v)
	{
		return matrix4x4 (1, 0, 0, 0, 
			0, 1, 0, 0, 
			0, 0, 1, 0, 
			v.x(), v.y(), v.z(), 1);
	}

	matrix4x4 rotation_matrix_x(real const& ang)
	{
		return matrix4x4(1, 0, 0, 0, 
					0, cos(ang), sin(ang), 0,
					0, -sin(ang), cos(ang), 0,
					0, 0, 0, 1);
	}

	matrix4x4 rotation_matrix_y(real const& ang)
	{
		return matrix4x4(cos(ang), 0, -sin(ang), 0,
					0, 1, 0, 0,
					sin(ang), 0, cos(ang), 0,
					0, 0, 0, 1);

	}

	matrix4x4 rotation_matrix_z(real const& ang)
	{
		return matrix4x4(cos(ang), sin(ang), 0, 0,
					-sin(ang), cos(ang), 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1);
	}

	matrix4x4 rotation_matrix(vector3 const& axis, real const& ang)
	{
		// TODO: need to implement this
		assert(false);
		return matrix4x4();
	}

	matrix4x4 scale_matrix(vector3 const& v)
	{
		return matrix4x4(v.x(), 0, 0, 0, 0, v.y(), 0, 0, 0, 0, v.z(), 0, 0, 0, 0, 1);
	}

	matrix4x4 perspective_proj_matrix(real const& l, real const& r, real const& b, real const& t, real const& n, real const& f)
	{
		return matrix4x4(2*n/(r-l), 0, 0, 0, 
						 0, 2*n/(t-b), 0, 0,
						 -(r + l)/(r - l), -(t + b)/(t - b), f/(f - n), 1,
						 0, 0, -f*n/(f - n), 0);  
	}

	matrix4x4 perspective_proj_fovy_matrix(real const& fovy, real const& aspect, real const& n, real const& f)
	{
		real hH = tan(fovy) * n;
		real hW  = hH * aspect;
		return perspective_proj_matrix( -hW, hW, -hH, hH, n, f);
	}

	matrix4x4 lookat_matrix( vector3 const& pos, vector3 const& at, vector3 const& up)
	{
		/// TODO: implement this
		assert(false);
		return matrix4x4();
	}


	quat rotation_quat(vector4 const& axis, real const& angle )
	{
		// create (sin(a/2)*axis, cos(a/2)) quaternion
		// which rotates the point a radians around "axis"
		quat res;
		vector4 u = axis; u.normalize();
		real sina2 = sin(angle/2);
		real cosa2 = cos(angle/2);

		res.qx() = sina2 * u.x();
		res.qy() = sina2 * u.y();
		res.qz() = sina2 * u.z();
		res.qw() = cosa2;

		return res;
	}

	vector4 rotate_vector( vector4 const& v, quat const& q )
	{
		quat p = quat(v);
		quat tp = q * p * q.inverse();
		return vector4(tp.qx(), tp.qy(), tp.qz(), tp.qw());
	}
}