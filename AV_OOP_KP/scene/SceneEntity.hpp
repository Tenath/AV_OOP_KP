#pragma once
#include <vector>
#include "..\math\av_vector_t.hpp"
#include "..\math\av_matrix_t.hpp"
#include "Material.hpp"
#include "Transform.hpp"
//#include "Model.hpp"
#include "Mesh.hpp"

namespace av
{
	class Mesh;

	class SceneEntity
	{
		Material* material = nullptr;
		Transform transform;
		Vector4f color = { 1.0f,1.0f,1.0f,0.0f };
		Mesh* mesh = nullptr;

		//Matrix4f transform;

		SceneEntity* parent = nullptr;
		std::vector<SceneEntity*> children;
	public:
		SceneEntity(Mesh* p_model, Vector3f p_pos) : mesh(p_model)
		{
			transform.SetPosition(p_pos);
			UpdateTransform(); 
		}

		SceneEntity()
		{

		}

		void Draw();
		void UpdateTransform();
		void UpdateUniforms();

		Mesh* GetModel() { return mesh; }
		void SetModel(Mesh* mod) { mesh = mod; }

		Vector4f GetPosition() { return transform.GetPosition(); }
		void SetPosition(Vector3f pos) { transform.SetPosition(pos); }

		Vector3f GetRotation() { return transform.GetRotation(); }
		void SetRotation(Vector3f rot) { transform.SetRotation(rot); }

		float GetScale() { return transform.GetScale(); }
		void SetScale(float sc) { transform.SetScale(sc); UpdateTransform(); }

		Vector4f GetColor() { return color; }
		void SetColor(Vector4f clr) { color = clr; }

		Material* GetMaterial() { return material; }
		void SetMaterial(Material* mat) { material = mat; }
	};

	/*class MeshEntity : public SceneEntity
	{
		Model* mesh = nullptr;
	};*/
}