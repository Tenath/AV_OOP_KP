#pragma once
#include "../math/av_matrix_t.hpp"
#include "../math/av_vector_t.hpp"

namespace av
{
	class Transform
	{
	private:
		Vector4f position;
		Vector3f rotation;
		float scale = 1.0f;

		Matrix4f matrix;

	public:
		Vector4f GetPosition() { return position; }
		void SetPosition(Vector4f pos) { position = pos; UpdateTransform(); }

		Vector3f GetRotation() { return rotation; }
		void SetRotation(Vector3f rot) { rotation = rot; UpdateTransform(); }

		float GetScale() { return scale; }
		void SetScale(float sc) { scale = sc; UpdateTransform(); }

		void UpdateTransform();

		const Matrix4f& GetMatrix() { return matrix; }
	};

	Matrix4f GenerateTranslationMatrix4f(const Vector4f& vec);
	Matrix4f GenerateRotationMatrix4f(float phi, float theta, float psi);
	Matrix4f GenerateScaleMatrix4f(float scale);
}