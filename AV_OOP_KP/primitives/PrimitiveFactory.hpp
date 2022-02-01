#pragma once
#include "../scene/Mesh.hpp"
#include "../scene/SceneEntity.hpp"
#include "../scene/SceneEntityBuilder.hpp"

namespace av
{
	class PrimitiveFactory
	{
	protected:
		Mesh* base_mesh = nullptr;
		VertexArray<Vertex, ushort>* va = nullptr;

		virtual void PrepareBaseMesh()
		{
			if (base_mesh != nullptr) return;
			base_mesh = GenerateMesh();
			if(va!=nullptr) base_mesh->SetupVertexArray(*va);
		}

		Mesh* GetBaseMesh() 
		{
			if (base_mesh == nullptr) PrepareBaseMesh();
			return base_mesh;
		}
	public:
		PrimitiveFactory(VertexArray<Vertex, ushort>& p_va)
		{
			va = &p_va;
		}

		virtual Mesh* GenerateMesh() = 0;
		virtual SceneEntityBuilder Build() = 0;
		virtual std::string GetPrimitiveName() = 0;

		virtual ~PrimitiveFactory()
		{
			if (base_mesh != nullptr) delete base_mesh;
		}
	};
}