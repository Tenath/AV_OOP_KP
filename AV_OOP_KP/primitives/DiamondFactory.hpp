#pragma once
#include "PrimitiveFactory.hpp"

namespace av
{
	class DiamondFactory : public PrimitiveFactory
	{
		size_t counter = 0;
	public:
		DiamondFactory(VertexArray<Vertex, ushort>& va) : PrimitiveFactory(va) {}

		Mesh* GenerateMesh() override;
		SceneEntityBuilder Build() override;
		std::string GetPrimitiveName() override;
	};
}