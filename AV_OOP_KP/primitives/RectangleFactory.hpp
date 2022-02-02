#pragma once
#include "PrimitiveFactory.hpp"

namespace av
{
	class RectangleFactory : public PrimitiveFactory
	{
		size_t counter = 0;
	public:
		RectangleFactory(VertexArray<Vertex, ushort>& va) : PrimitiveFactory(va) {}

		Mesh* GenerateMesh() override;
		SceneEntityBuilder Build() override;
		std::string GetPrimitiveName() override;
	};
}