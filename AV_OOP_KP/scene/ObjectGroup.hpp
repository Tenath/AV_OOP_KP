#pragma once
#include <string>
#include "..\math\av_vector_t.hpp"

namespace av
{
	class ObjectGroup
	{
		std::string name;
		Vector4f position;
		Vector3f rotation;
		float scale = 1.0f;

	public:
		ObjectGroup(std::string p_name, Vector4f p_pos, Vector3f p_rot, float p_scale = 1.0f) :
			name(p_name),
			position(p_pos),
			rotation(p_rot),
			scale(p_scale) {}

		std::string GetName() { return name; }
		Vector4f GetPosition() { return position; }
		Vector3f GetRotation() { return rotation; }
		float GetScale() { return scale; }

		void SetName(const std::string& p_name) { name = p_name; }
		void SetPosition(const Vector4f& p_pos) { position = p_pos; }
		void SetRotation(const Vector3f& p_rot) { rotation = p_rot; }
		void SetScale(float p_scale) { scale = p_scale; }
	};
}