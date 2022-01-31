#pragma once
#include "PrimitiveBase.hpp"

namespace av
{
	class TetrahedronBase : public PrimitiveBase
	{
		Mesh* mesh = nullptr;
		size_t counter = 0;
	public:
		TetrahedronBase(VertexArray<Vertex, ushort>& va) : PrimitiveBase(va) {}

		Mesh* GenerateMesh() override;
		SceneEntityBuilder Build() override;
		std::string GetPrimitiveName() override;
	};
}