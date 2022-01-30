#pragma once
#include "PrimitiveBase.hpp"

namespace av
{
	class CubeBase : public PrimitiveBase
	{
		Mesh* mesh = nullptr;
		size_t counter = 0;
	public:
		CubeBase(VertexArray<Vertex, ushort>& va) : PrimitiveBase(va) {}

		Mesh* GenerateMesh() override;
		SceneEntityBuilder Build() override;
	};
}