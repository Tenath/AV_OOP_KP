#pragma once
#include "PrimitiveFactory.hpp"

namespace av
{
	class CubeFactory : public PrimitiveFactory
	{
		size_t counter = 0;
	public:
		CubeFactory(VertexArray<Vertex, ushort>& va) : PrimitiveFactory(va) {}

		Mesh* GenerateMesh() override;
		SceneEntityBuilder Build() override;
		std::string GetPrimitiveName() override;
	};
}