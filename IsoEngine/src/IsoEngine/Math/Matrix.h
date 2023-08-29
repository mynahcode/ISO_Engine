#pragma once

namespace IE
{
	class IsoMatrix3D
	{
	private:

	public:
		IsoMatrix3D();

		// Column Major Order -- OpenGL's preference: 
		IsoMatrix3D(float m0, float m3, float m6,		// 0   3   6
			float m1, float m4, float m7,		// 1   4   7
			float m2, float m5, float m8);		// 2   5   8


		~IsoMatrix3D();

		// Copy Constructors
		IsoMatrix3D& operator=(const IsoMatrix3D& matrix);

		// Operator Overloads
		IsoMatrix3D& operator+=(const IsoMatrix3D& matrix);
		IsoMatrix3D& operator-=(const IsoMatrix3D& matrix);
		IsoMatrix3D& operator*=(const float scalar); // Scalar Multiplication
		IsoMatrix3D& operator/=(const float scalar); // Scalar Division

		IsoMatrix3D& operator+(const IsoMatrix3D& matrix) const;
		IsoMatrix3D& operator-(const IsoMatrix3D& matrix) const;
		IsoMatrix3D& operator*(const float scalar) const; // Scalar Multiplication
		IsoMatrix3D& operator/(const float scalar) const; // Scalar Division

		void Print();

		float matrixData[9] = { 0.0f };
	};
}