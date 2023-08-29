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

	IsoMatrix3D& IsoMatrix3D::operator+=(const IsoMatrix3D& matrix)
	{
		matrixData[0] += matrix.matrixData[0];
		matrixData[3] += matrix.matrixData[3];
		matrixData[6] += matrix.matrixData[6];

		matrixData[1] += matrix.matrixData[1];
		matrixData[4] += matrix.matrixData[4];
		matrixData[7] += matrix.matrixData[7];

		matrixData[2] += matrix.matrixData[2];
		matrixData[5] += matrix.matrixData[5];
		matrixData[8] += matrix.matrixData[8];

		return *this;
	}

	IsoMatrix3D& IsoMatrix3D::operator-=(const IsoMatrix3D& matrix)
	{
		matrixData[0] -= matrix.matrixData[0];
		matrixData[3] -= matrix.matrixData[3];
		matrixData[6] -= matrix.matrixData[6];

		matrixData[1] -= matrix.matrixData[1];
		matrixData[4] -= matrix.matrixData[4];
		matrixData[7] -= matrix.matrixData[7];

		matrixData[2] -= matrix.matrixData[2];
		matrixData[5] -= matrix.matrixData[5];
		matrixData[8] -= matrix.matrixData[8];

		return *this;
	}

	IsoMatrix3D& IsoMatrix3D::operator*=(const float scalar)
	{
		matrixData[0] *= scalar;
		matrixData[3] *= scalar;
		matrixData[6] *= scalar;

		matrixData[1] *= scalar;
		matrixData[4] *= scalar;
		matrixData[7] *= scalar;

		matrixData[2] *= scalar;
		matrixData[5] *= scalar;
		matrixData[8] *= scalar;

		return *this;
	}

	IsoMatrix3D& IsoMatrix3D::operator/=(const float scalar)
	{
		matrixData[0] /= scalar;
		matrixData[3] /= scalar;
		matrixData[6] /= scalar;

		matrixData[1] /= scalar;
		matrixData[4] /= scalar;
		matrixData[7] /= scalar;

		matrixData[2] /= scalar;
		matrixData[5] /= scalar;
		matrixData[8] /= scalar;

		return *this;
	}

	IsoMatrix3D& IsoMatrix3D::operator+(const IsoMatrix3D& matrix) const
	{
		IsoMatrix3D nMatrix;

		nMatrix.matrixData[0] = matrixData[0] + matrix.matrixData[0];
		nMatrix.matrixData[3] = matrixData[3] + matrix.matrixData[3];
		nMatrix.matrixData[6] = matrixData[6] + matrix.matrixData[6];

		nMatrix.matrixData[1] = matrixData[1] + matrix.matrixData[1];
		nMatrix.matrixData[4] = matrixData[4] + matrix.matrixData[4];
		nMatrix.matrixData[7] = matrixData[7] + matrix.matrixData[7];

		nMatrix.matrixData[2] = matrixData[2] + matrix.matrixData[2];
		nMatrix.matrixData[5] = matrixData[5] + matrix.matrixData[5];
		nMatrix.matrixData[8] = matrixData[8] + matrix.matrixData[8];

		return nMatrix;
	}

	IsoMatrix3D& IsoMatrix3D::operator-(const IsoMatrix3D& matrix) const
	{
		IsoMatrix3D nMatrix;

		nMatrix.matrixData[0] = matrixData[0] - matrix.matrixData[0];
		nMatrix.matrixData[3] = matrixData[3] - matrix.matrixData[3];
		nMatrix.matrixData[6] = matrixData[6] - matrix.matrixData[6];

		nMatrix.matrixData[1] = matrixData[1] - matrix.matrixData[1];
		nMatrix.matrixData[4] = matrixData[4] - matrix.matrixData[4];
		nMatrix.matrixData[7] = matrixData[7] - matrix.matrixData[7];

		nMatrix.matrixData[2] = matrixData[2] - matrix.matrixData[2];
		nMatrix.matrixData[5] = matrixData[5] - matrix.matrixData[5];
		nMatrix.matrixData[8] = matrixData[8] - matrix.matrixData[8];

		return nMatrix;
	}

	IsoMatrix3D& IsoMatrix3D::operator*(float scalar) const
	{
		IsoMatrix3D nMatrix;

		nMatrix.matrixData[0] = matrixData[0] * scalar;
		nMatrix.matrixData[3] = matrixData[3] * scalar;
		nMatrix.matrixData[6] = matrixData[6] * scalar;

		nMatrix.matrixData[1] = matrixData[1] * scalar;
		nMatrix.matrixData[4] = matrixData[4] * scalar;
		nMatrix.matrixData[7] = matrixData[7] * scalar;

		nMatrix.matrixData[2] = matrixData[2] * scalar;
		nMatrix.matrixData[5] = matrixData[5] * scalar;
		nMatrix.matrixData[8] = matrixData[8] * scalar;

		return nMatrix;
	}

	IsoMatrix3D& IsoMatrix3D::operator/(float scalar) const
	{
		IsoMatrix3D nMatrix;

		nMatrix.matrixData[0] = matrixData[0] / scalar;
		nMatrix.matrixData[3] = matrixData[3] / scalar;
		nMatrix.matrixData[6] = matrixData[6] / scalar;

		nMatrix.matrixData[1] = matrixData[1] / scalar;
		nMatrix.matrixData[4] = matrixData[4] / scalar;
		nMatrix.matrixData[7] = matrixData[7] / scalar;

		nMatrix.matrixData[2] = matrixData[2] / scalar;
		nMatrix.matrixData[5] = matrixData[5] / scalar;
		nMatrix.matrixData[8] = matrixData[8] / scalar;

		return nMatrix;
	}

	void IsoMatrix3D::Print()
	{
		std::cout << "[" << matrixData[0] << "," << matrixData[3] << "," << matrixData[6] << "," << std::endl;
		std::cout << matrixData[1] << "," << matrixData[4] << "," << matrixData[7] << "," << std::endl;
		std::cout << matrixData[2] << "," << matrixData[5] << "," << matrixData[8] << "]" << std::endl;
	}
}