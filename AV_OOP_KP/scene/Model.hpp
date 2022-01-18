#pragma once
#include <string>
#include "../globj/Vertex.hpp"
#include "../globj/VertexArray.hpp"
#include "Mesh.hpp"
#include "SceneEntity.hpp"

namespace av
{
	class Model : BindableGLObject
	{
	public:
		virtual ~Model() = 0;
		virtual void Draw() = 0;
	};
}