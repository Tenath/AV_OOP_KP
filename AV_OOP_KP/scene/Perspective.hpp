#pragma once
#include "../math/av_matrix_t.hpp"

namespace av
{
	class Perspective
	{
		Matrix4f proj;
		float FrustumScaleX = 1.0f;
		float FrustumScaleY = 1.0f;

		float FOV = 45.0f;
		float AspectRatio = 1.333f;
	public:

		float zNear = 1.0f;
		float zFar = 10000.0f;

		Perspective();

		Matrix4f& GetProjection() noexcept;

		void UpdateScale();

		void SetFOV(float pFOV);

		void SetAspectRatio(float ratio);

		void UpdateProjection();
	};
}