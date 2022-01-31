#pragma once
#include "PrimitiveBase.hpp"

namespace av
{
	class SphereBase : public PrimitiveBase
	{
		Mesh* mesh = nullptr;
		size_t counter = 0;
	public:
		SphereBase(VertexArray<Vertex, ushort>& va) : PrimitiveBase(va) {}

		Mesh* GenerateMesh() override;
		SceneEntityBuilder Build() override;
	};
}