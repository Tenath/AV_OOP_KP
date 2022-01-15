#include "Perspective.hpp"

namespace av
{
	Perspective::Perspective()
	{
		UpdateScale();
	}

	Matrix4f& Perspective::GetProjection() noexcept { return proj; }

	void Perspective::UpdateScale()
	{
		FrustumScaleY = 2.0f * zNear / std::tan(DegToRad(FOV) / 2.0f);
		FrustumScaleX = FrustumScaleY / AspectRatio;
	}

	void Perspective::SetFOV(float pFOV)
	{
		FOV = pFOV;
		UpdateScale();
	}

	void Perspective::SetAspectRatio(float ratio)
	{
		AspectRatio = ratio;
		UpdateScale();
	}

	void Perspective::UpdateProjection()
	{
		proj(0, 0) = FrustumScaleX;
		proj(1, 1) = FrustumScaleY;
		proj(2, 2) = (zNear + zFar) / (zNear - zFar);
		proj(2, 3) = -1.0f;
		proj(3, 2) = (2.0f * zFar * zNear) / (zNear - zFar);;
	}
}