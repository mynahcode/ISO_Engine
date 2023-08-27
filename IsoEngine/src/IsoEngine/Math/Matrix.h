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

		float matrixData[9] = { 0.0f };
	};
}