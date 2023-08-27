#pragma once

namespace IE
{
	class IsoVector3D
	{
	private:

	public:

		IsoVector3D();
		IsoVector3D(float mX, float mY, float mZ);

		~IsoVector3D();

		// Copy Constructors
		IsoVector3D(const IsoVector3D& vec);
		IsoVector3D& operator=(const IsoVector3D& vec);

		// Operator Overloads
		IsoVector3D& operator+=(const IsoVector3D& vec);
		IsoVector3D& operator-=(const IsoVector3D& vec);
		IsoVector3D& operator*=(const float scalar);
		IsoVector3D& operator/=(const float scalar);
		IsoVector3D& operator+(const IsoVector3D& vec) const;
		IsoVector3D& operator-(const IsoVector3D& vec) const; 
		IsoVector3D& operator*(const float scalar) const; // Scalar (float) multiplication for vectors.
		IsoVector3D& operator/(const float scalar) const;

		// Dot-Product Operator & Function -- mainly used to determine the angle between vectors.
		float operator*(const IsoVector3D& vec) const;
		float dot(const IsoVector3D& vec) const;

		// Cross-Product Operator & Function -- two vectors produce a plane; cross-products of two vectors produces a vector perpendicular to two given vectors.
		IsoVector3D cross(const IsoVector3D& vec) const;
		IsoVector3D operator%(const IsoVector3D& vec) const;
		void operator%=(const IsoVector3D& vec);

		float x, y, z;
	};
}