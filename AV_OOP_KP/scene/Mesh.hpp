#pragma once
#include <vector>
#include <string>
#include "..\globals.hpp"
#include "..\globj\VertexArray.hpp"
#include "..\globj\Vertex.hpp"
#include "..\globj\VertexGroup.hpp"
#include "Material.hpp"
#include "Model.hpp"
#include "..\globj\Program.hpp"

namespace av
{
	// Mesh = vertices + indices
	// can be attached to VertexArray
	class Mesh : public Model
	{
		std::string name;
		VertexGroup<Vertex>* vertices = nullptr;
		IndexGroup<ushort>* indices = nullptr;
		Material* material = nullptr;

		//VertexArrayPointer<Vertex, ushort> va_ptr;
		VertexArray<Vertex, ushort>* va = nullptr;
		DrawSequence<Vertex, ushort>* ds = nullptr;

		void CleanupVA()
		{
			if (va != nullptr)
			{
				va->RemoveDS(ds);
				va = nullptr;
			}
		}
	public:

		Mesh(
			const std::string& p_name,
			const std::vector<Vertex>& p_vertices, 
			const std::vector<ushort>& p_indices,
			PrimitiveType p_type,
			Material* mt,
			VertexArray<Vertex, ushort>* varray = nullptr
		)
		{
			name = p_name;
			vertices = new VertexGroup<Vertex>(p_name + ".vertices", p_vertices);
			indices = new IndexGroup<ushort>(p_name + ".indices", p_indices, p_type);
			material = mt;

			if (varray != nullptr) SetupVertexArray(*varray);
		}

		~Mesh() override
		{
			CleanupVA();
			if (ds != nullptr) delete ds;
			if (vertices != nullptr) delete vertices;
			if (indices != nullptr) delete indices;
		}

		void SetupVertexArray(VertexArray<Vertex, ushort>& p_va)
		{
			ds = new DrawSequence<Vertex, ushort>(vertices, indices);
			va = &p_va;
			va->AddDS(ds);
			//va_ptr = VertexArrayPointer<Vertex, ushort>::Setup(va, *vertices, *indices);
		}

		void Bind() override
		{
			if(material != nullptr) material->GetProgram()->Bind();
			if (va != nullptr) va->Bind();
		}

		void Draw() override
		{
			if (ds != nullptr)
			{
				Bind();
				ds->Draw();
			}
		}

		std::string GetName() override
		{
			return name;
		}
	};
}