#pragma once
#include <vector>
#include "..\math\av_vector_t.hpp"
#include "..\math\av_matrix_t.hpp"
#include "Material.hpp"
#include "Transform.hpp"
//#include "Model.hpp"
#include "Model.hpp"

namespace av
{
	class Model;
	class Scene;

	class SceneEntity
	{
		std::string name;

		Material* material = nullptr;
		Transform transform;
		Vector4f color = { 1.0f,1.0f,1.0f,0.0f };
		Model* model = nullptr;
		Scene* scene = nullptr;

		//Matrix4f transform;

		SceneEntity* parent = nullptr;
		std::vector<SceneEntity*> children;
	public:
		SceneEntity(Model* p_model, Vector3f p_pos) : model(p_model)
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

		std::string& EditName() { return name; }
		std::string GetName() { return name; }
		void SetName(const std::string& p_name) { name = p_name; }

		Model* GetModel() { return model; }
		void SetModel(Model* mod) { model = mod; }

		Vector4f GetPosition() { return transform.GetPosition(); }
		void SetPosition(Vector3f pos) { transform.SetPosition(pos); }

		Vector3f GetRotation() { return transform.GetRotation(); }
		void SetRotation(Vector3f rot) { transform.SetRotation(rot); }

		float GetScale() { return transform.GetScale(); }
		void SetScale(float sc) { transform.SetScale(sc); UpdateTransform(); }

		Vector4f& EditColor() { return color; }
		Vector4f GetColor() { return color; }
		void SetColor(Vector4f clr) { color = clr; }

		Material* GetMaterial() { return material; }
		void SetMaterial(Material* mat) { material = mat; }

		Scene* GetScene() { return scene; }
		void SetScene(Scene* sc);

		Transform& GetTransform() { return transform; }
	};
}