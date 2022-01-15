#pragma once
#include "../math/av_vector_t.hpp"
#include "../math/av_matrix_t.hpp"

namespace av
{
	class LightSource
	{
	private:
		unsigned index;

		Vector3f position;

		//Vector3f color;
		Vector3f ambient;
		Vector3f diffuse;
		Vector3f specular;

		float constant;
		float linear;
		float quadratic;

		Matrix4f transform;
		//void UpdateTransform();
	public:
		unsigned GetIndex() { return index; }
		void SetIndex(unsigned i) { index = i; }

		Vector3f GetPosition() { return position; }
		void SetPosition(Vector3f pos) { position = pos; }
		void AdjustPosition(Vector3f pos) { position += pos; }

		//Vector3f GetColor() { return color; }
		//void SetColor(Vector3f clr) { color = clr; }

		Vector3f GetAmbient() { return ambient; }
		Vector3f GetDiffuse() { return diffuse; }
		Vector3f GetSpecular() { return specular; }

		void SetAmbient(Vector3f clr) { ambient = clr; }
		void SetDiffuse(Vector3f clr) { diffuse = clr; }
		void SetSpecular(Vector3f clr) { specular = clr; }
		void SetParameters(float c, float l, float q)
		{
			constant = c;
			linear = l;
			quadratic = q;
		}

		void UpdateUniforms(const std::string& name);
	};
}