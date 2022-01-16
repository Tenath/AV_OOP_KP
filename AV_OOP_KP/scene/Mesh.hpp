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
		VertexGroup<Vertex>* vertices;
		IndexGroup<ushort>* indices;
		Material* material;

		VertexArrayPointer<Vertex, ushort> va_ptr;
	};
}