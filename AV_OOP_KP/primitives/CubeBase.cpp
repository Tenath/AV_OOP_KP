#include "CubeBase.hpp"
#include "../globj/Vertex.hpp"
#include "../scene/Mesh.hpp"

namespace av
{
	Mesh* CubeBase::GenerateMesh()
	{
		std::vector<Vertex> vertices =
		{
			Vertex(Vector3f(-1.0f, -1.0f, -1.0f), Vector4f(1.0f,1.0f,1.0f, 1.0f)),
			Vertex(Vector3f(-1.0f, -1.0f, 1.0f), Vector4f(1.0f,1.0f,1.0f, 1.0f)),
			Vertex(Vector3f(-1.0f, 1.0f, -1.0f), Vector4f(1.0f,1.0f,1.0f, 1.0f)),
			Vertex(Vector3f(-1.0f, 1.0f, 1.0f), Vector4f(1.0f,1.0f,1.0f, 1.0f)),
			Vertex(Vector3f(1.0f, -1.0f, -1.0f), Vector4f(1.0f,1.0f,1.0f, 1.0f)),
			Vertex(Vector3f(1.0f, -1.0f, 1.0f), Vector4f(1.0f,1.0f,1.0f, 1.0f)),
			Vertex(Vector3f(1.0f, 1.0f, -1.0f), Vector4f(1.0f,1.0f,1.0f, 1.0f)),
			Vertex(Vector3f(1.0f, 1.0f, 1.0f), Vector4f(1.0f,1.0f,1.0f, 1.0f)),
		};

		std::vector<ushort> indices = { 
			// sides as seen looking from +x direction
			0,1,2, 1,2,3, // back side
			0,2,6, 0,4,6, // left side
			1,3,7, 1,5,7, // right side
			4,7,6, 4,5,7, // front side
			2,6,7, 2,7,3, // top side
			0,4,5, 0,1,5  // bottom side
		};

		PrimitiveType type = PrimitiveType::Triangles;

		Mesh* tri_mesh = new Mesh(
			"Cube",
			vertices,
			indices,
			PrimitiveType::Triangles,
			nullptr,
			nullptr
		);

		return tri_mesh;
	}

	SceneEntityBuilder CubeBase::Build()
	{
		SceneEntityBuilder sb;
		sb.WithMesh(*GetBaseMesh());
		sb.WithName("Cube #" + std::to_string(++counter));
		return sb;
	}

	std::string CubeBase::GetPrimitiveName()
	{
		return "Cube";
	}
}