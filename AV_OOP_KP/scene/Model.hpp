#pragma once
#include <string>
#include "../globj/Vertex.hpp"
#include "../globj/VertexArray.hpp"
#include "SceneEntity.hpp"

namespace av
{
	class Model
	{
	public:
		virtual ~Model() {};
		virtual void Bind() = 0;
		virtual void Draw() = 0;
	};
}