#pragma once
#include "PrimitiveFactory.hpp"

namespace av
{
	class TetrahedronFactory : public PrimitiveFactory
	{
		size_t counter = 0;
	public:
		TetrahedronFactory(VertexArray<Vertex, ushort>& va) : PrimitiveFactory(va) {}

		Mesh* GenerateMesh() override;
		SceneEntityBuilder Build() override;
		std::string GetPrimitiveName() override;
	};
}