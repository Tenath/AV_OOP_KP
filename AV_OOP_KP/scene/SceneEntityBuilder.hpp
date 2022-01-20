#pragma once
#include <stdexcept>
#include "SceneEntity.hpp"
#include "Mesh.hpp"

namespace av
{
	class SceneEntityBuilder
	{
		SceneEntity* obj = nullptr;

	public:
		SceneEntityBuilder() noexcept {}

		SceneEntityBuilder(SceneEntityBuilder&& temp) noexcept
		{
			this->obj = temp.obj;
			temp.obj = nullptr;
		}

		SceneEntityBuilder& operator=(SceneEntityBuilder&& temp)
		{
			this->obj = temp.obj;
			temp.obj = nullptr;

			return *this;
		}

		SceneEntityBuilder& Begin()
		{
			if (obj != nullptr)
				throw std::logic_error("Builder process already started");

			obj = new SceneEntity();
			return *this;
		}

		~SceneEntityBuilder()
		{
			if (obj != nullptr) delete obj;
		}

		SceneEntityBuilder& WithMesh(Mesh& mesh)
		{
			if (obj == nullptr) Begin();
			obj->SetModel(&mesh);
			return *this;
		}

		SceneEntityBuilder& Position(Vector3f pos)
		{
			if (obj == nullptr) Begin();
			obj->SetPosition(pos);
			return *this;
		}

		SceneEntityBuilder& Rotate(Vector3f rot)
		{
			if (obj == nullptr) Begin();
			obj->SetRotation(rot);
			return *this;
		}

		SceneEntityBuilder& Scale(float scale)
		{
			if (obj == nullptr) Begin();
			obj->SetScale(scale);
			return *this;
		}

		SceneEntityBuilder& WithMaterial(Material& mat)
		{
			if (obj == nullptr) Begin();
			obj->SetMaterial(&mat);
			return *this;
		}

		SceneEntityBuilder& WithColor(Vector4f color)
		{
			if (obj == nullptr) Begin();
			obj->SetColor(color);
			return *this;
		}

		SceneEntity* Finish() noexcept
		{
			SceneEntity* result = obj;
			obj = nullptr;
			return result;
		}
	};
}