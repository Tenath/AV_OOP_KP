#include "RectangleFactory.hpp"
#include "../globj/Vertex.hpp"
#include "../scene/Mesh.hpp"

namespace av
{
	Mesh* RectangleFactory::GenerateMesh()
	{
		std::vector<Vertex> vertices =
		{
			Vertex(Vector3f(-0.5f, -0.5f, 0.0f), Vector4f(1.0f,1.0f,1.0f, 1.0f)),
			Vertex(Vector3f(-0.5f, 0.5f, 0.0f), Vector4f(1.0f,1.0f,1.0f, 1.0f)),
			Vertex(Vector3f(0.5f, -0.5f, 0.0f), Vector4f(1.0f,1.0f,1.0f, 1.0f)),
			Vertex(Vector3f(0.5f, 0.5f, 0.0f), Vector4f(1.0f,1.0f,1.0f, 1.0f)),
		};

		std::vector<ushort> indices = {
			0,1,2, 1,2,3
		};

		PrimitiveType type = PrimitiveType::Triangles;

		Mesh* tri_mesh = new Mesh(
			"Rectangle",
			vertices,
			indices,
			PrimitiveType::Triangles,
			nullptr,
			nullptr
		);

		return tri_mesh;
	}

	SceneEntityBuilder RectangleFactory::Build()
	{
		SceneEntityBuilder sb;
		sb.WithMesh(*GetBaseMesh());
		sb.WithName("Rectangle #" + std::to_string(++counter));
		return sb;
	}

	std::string RectangleFactory::GetPrimitiveName()
	{
		return "Rectangle";
	}
}