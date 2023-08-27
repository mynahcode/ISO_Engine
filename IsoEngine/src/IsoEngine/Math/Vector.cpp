#include "iepch.h"

#include "Vector.h"

namespace IE
{
	IsoVector3D::IsoVector3D()
		: x(0.0f), y(0.0f), z(0.0f) {}

	IsoVector3D::IsoVector3D(float mX, float mY, float mZ)
		: x(mX), y(mY), z(mZ) {}

	IsoVector3D::~IsoVector3D() {}

	IsoVector3D::IsoVector3D(const IsoVector3D& vec)
		: x(vec.x), y(vec.y), z(vec.z) {}

	IsoVector3D& IsoVector3D::operator=(const IsoVector3D& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;

		return *this;
	}

	IsoVector3D& IsoVector3D::operator+=(const IsoVector3D& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}

	IsoVector3D& IsoVector3D::operator-=(const IsoVector3D& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}

	IsoVector3D& IsoVector3D::operator*=(const float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}

	IsoVector3D& IsoVector3D::operator/=(const float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
	}

	/* Vector Addidtion/Subtraction */
	IsoVector3D& IsoVector3D::operator+(const IsoVector3D& vec) const
	{
		return IsoVector3D(x + vec.x, y + vec.y, z + vec.z);
	}

	IsoVector3D& IsoVector3D::operator-(const IsoVector3D& vec) const
	{
		return IsoVector3D(x - vec.x, y - vec.y, z - vec.z);
	}

	/* Scalar Multiplication/Division on vectors */
	IsoVector3D& IsoVector3D::operator*(const float scalar) const
	{
		return IsoVector3D(x * scalar, y * scalar, z * scalar);
	}

	IsoVector3D& IsoVector3D::operator/(const float scalar) const
	{
		return IsoVector3D(x / scalar, y / scalar, z / scalar);
	}

	/* Dot Product */
	float IsoVector3D::operator*(const IsoVector3D& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}

	float IsoVector3D::dot(const IsoVector3D& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}

	/* Cross Product */

	IsoVector3D IsoVector3D::cross(const IsoVector3D& vec) const
	{
		return IsoVector3D(y * vec.z - z * vec.y,
						   z * vec.x - x * vec.z,
						   x * vec.y - y * vec.x);
	}

	IsoVector3D IsoVector3D::operator%(const IsoVector3D& vec) const
	{
		return IsoVector3D(y * vec.z - z * vec.y,
						   z * vec.x - x * vec.z,
						   x * vec.y - y * vec.x);
	}

	void IsoVector3D::operator%=(const IsoVector3D& vec)
	{
		*this = cross(vec);
	}
}