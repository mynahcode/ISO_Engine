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
		IsoVector3D& operator/(const float scalar) const; // Scalar (float) division for vectors.

		// Dot-Product Operator & Function -- mainly used to determine the angle between vectors.
		float operator*(const IsoVector3D& vec) const;
		float Dot(const IsoVector3D& vec) const;

		// Cross-Product Operator & Function -- two vectors produce a plane; cross-products of two vectors produces a vector perpendicular to two given vectors.
		IsoVector3D Cross(const IsoVector3D& vec) const;
		IsoVector3D operator%(const IsoVector3D& vec) const;
		void operator%=(const IsoVector3D& vec);

		// Methods for obtaining information/operating on just the vector itself.
		float Magnitude(); // Determining the length of a vector
		void Normalize(); // For normalizing a vector -- obtaining a unit vector.
		void Print(); // Output the vector

		float x, y, z;
	};
}