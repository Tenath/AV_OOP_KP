#pragma once
#include "PrimitiveBase.hpp"

namespace av
{
	class TriangleBase : public PrimitiveBase
	{
		Mesh* mesh=nullptr;
	public:
		TriangleBase(VertexArray<Vertex, ushort>& va) : PrimitiveBase(va) {}

		Mesh* GenerateMesh() override;
		SceneEntityBuilder Build() override;
	};
}