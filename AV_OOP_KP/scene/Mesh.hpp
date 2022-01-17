#pragma once
#include <vector>
#include <string>
#include "..\globj\VertexArray.hpp"
#include "..\globj\Vertex.hpp"
#include "..\globj\VertexGroup.hpp"
#include "Material.hpp"
#include "..\globals.hpp"

namespace av
{
	// Mesh = vertices + indices
	// can be attached to VertexArray
	class Mesh
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

		Mesh()
		{

		}

		~Mesh()
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
	};
}