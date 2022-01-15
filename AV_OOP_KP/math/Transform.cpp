#include <cmath>
#include "av_matrix_t.hpp"
using namespace av;

Matrix3f GenerateRotationMatrix(float phi, float theta, float psi)
{
	Matrix3f xrot;
	xrot(0, 0) = 1.0f;
	xrot(1, 1) = cos(phi);
	xrot(1, 2) = -sin(phi);
	xrot(2, 1) = sin(phi);
	xrot(2, 2) = cos(phi);

	Matrix3f yrot;
	yrot(0, 0) = cos(theta);
	yrot(0, 2) = sin(theta);
	yrot(1, 1) = 1.0f;
	yrot(2, 0) = -sin(theta);
	yrot(2, 2) = cos(theta);

	Matrix3f zrot;
	zrot(0, 0) = cos(psi);
	zrot(0, 1) = -sin(psi);
	zrot(1, 0) = sin(psi);
	zrot(1, 1) = cos(psi);
	zrot(2, 2) = 1.0f;

	return xrot * yrot * zrot;
}