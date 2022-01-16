#pragma once
#include <string>
#include "../globj/Vertex.hpp"
#include "../globj/VertexArray.hpp"

namespace av
{
	class Model
	{
		VertexArray<Vertex, USHORT>* va;
		DrawSequence<Vertex, USHORT> ps;

	public:
		Model(const std::string& filename);

		~Model();

		void Draw();
	};
}