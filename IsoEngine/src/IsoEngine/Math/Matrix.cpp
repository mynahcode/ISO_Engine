#include "iepch.h"

#include "Matrix.h"

namespace IE
{
	/* Returns 3x3 Identity Matrix:
	 * 1 0 0
	 * 0 1 0
	 * 0 0 1
	 */
	IsoMatrix3D::IsoMatrix3D()
	{
		for (int i = 0; i < 9; i++)
			matrixData[i] = 0.0f;

		matrixData[0] = matrixData[4] = matrixData[8] = 1.0f;
	}

	// Column Major Order
	IsoMatrix3D::IsoMatrix3D(float m0, float m3, float m6,
							 float m1, float m4, float m7,
							 float m2, float m5, float m8)
	{
		matrixData[0] = m0;
		matrixData[3] = m3;
		matrixData[6] = m6;

		matrixData[1] = m1;
		matrixData[4] = m4;
		matrixData[7] = m7;

		matrixData[2] = m2;
		matrixData[5] = m5;
		matrixData[8] = m8;
	}


	IsoMatrix3D::~IsoMatrix3D() {}

	// Copy Constructors
	IsoMatrix3D& IsoMatrix3D::operator=(const IsoMatrix3D& matrix)
	{
		matrixData[0] = matrix.matrixData[0];
		matrixData[3] = matrix.matrixData[3];
		matrixData[6] = matrix.matrixData[6];

		matrixData[1] = matrix.matrixData[1];
		matrixData[4] = matrix.matrixData[4];
		matrixData[7] = matrix.matrixData[7];

		matrixData[2] = matrix.matrixData[2];
		matrixData[5] = matrix.matrixData[5];
		matrixData[8] = matrix.matrixData[8];

		return *this;
	}
}