#pragma once
#include "PrimitiveBase.hpp"

namespace av
{
	class TriangleBase : public PrimitiveBase
	{
		Mesh* mesh=nullptr;
		size_t counter = 0;
	public:
		TriangleBase(VertexArray<Vertex, ushort>& va) : PrimitiveBase(va) {}

		Mesh* GenerateMesh() override;
		SceneEntityBuilder Build() override;
		std::string GetPrimitiveName() override;
	};
}