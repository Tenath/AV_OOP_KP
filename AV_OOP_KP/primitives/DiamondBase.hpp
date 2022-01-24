#pragma once
#include "PrimitiveBase.hpp"

namespace av
{
	class DiamondBase : public PrimitiveBase
	{
		Mesh* mesh = nullptr;
	public:
		DiamondBase(VertexArray<Vertex, ushort>& va) : PrimitiveBase(va) {}

		Mesh* GenerateMesh() override;
		SceneEntityBuilder Build() override;
	};
}