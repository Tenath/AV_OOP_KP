#pragma once
#include "PrimitiveBase.hpp"

namespace av
{
	class RectangleBase : public PrimitiveBase
	{
		Mesh* mesh = nullptr;
		size_t counter = 0;
	public:
		RectangleBase(VertexArray<Vertex, ushort>& va) : PrimitiveBase(va) {}

		Mesh* GenerateMesh() override;
		SceneEntityBuilder Build() override;
		std::string GetPrimitiveName() override;
	};
}