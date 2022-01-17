#pragma once
#include <string>
#include "../globj/Vertex.hpp"
#include "../globj/VertexArray.hpp"
#include "Mesh.hpp"
#include "SceneEntity.hpp"

namespace av
{
	class Model
	{
		Mesh* mesh;
		//VertexArray<Vertex, USHORT>* va;
		//DrawSequence<Vertex, USHORT> ps;

	public:
		//Model(const std::string& filename);
		//Model(Mesh* mesh);

		virtual ~Model();

		virtual void Draw();
	};
}