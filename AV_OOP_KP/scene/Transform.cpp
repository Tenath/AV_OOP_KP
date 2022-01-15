#include <cmath>
#include "Transform.hpp"

namespace av
{
	Matrix4f GenerateTranslationMatrix4f(const Vector4f& vec)
	{
		Matrix4f mat = IdentityMatrixf<4>();

		mat(0, 3) = vec.X();
		mat(1, 3) = vec.Y();
		mat(2, 3) = vec.Z();

		return mat;
	}

	Matrix4f GenerateScaleMatrix4f(float scale)
	{
		Matrix4f mat = IdentityMatrixf<4>();

		mat(0, 0) = scale;
		mat(1, 1) = scale;
		mat(2, 2) = scale;

		return mat;
	}

	Matrix4f GenerateRotationMatrix4f(float phi, float theta, float psi)
	{
		Matrix4f xrot = IdentityMatrixf<4>();
		xrot(1, 1) = cos(phi);
		xrot(1, 2) = -sin(phi);
		xrot(2, 1) = sin(phi);
		xrot(2, 2) = cos(phi);

		Matrix4f yrot = IdentityMatrixf<4>();
		yrot(0, 0) = cos(theta);
		yrot(0, 2) = sin(theta);
		yrot(2, 0) = -sin(theta);
		yrot(2, 2) = cos(theta);

		Matrix4f zrot = IdentityMatrixf<4>();
		zrot(0, 0) = cos(psi);
		zrot(0, 1) = -sin(psi);
		zrot(1, 0) = sin(psi);
		zrot(1, 1) = cos(psi);

		return xrot * yrot * zrot;
	}

	void Transform::UpdateTransform()
	{ 
		matrix = GenerateTranslationMatrix4f(position) *
			GenerateRotationMatrix4f(rotation.X(), rotation.Y(), rotation.Z()) *
			GenerateScaleMatrix4f(scale);
	}
}