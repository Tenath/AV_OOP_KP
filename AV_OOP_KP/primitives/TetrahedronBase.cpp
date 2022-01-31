#include "TetrahedronBase.hpp"
#include "../globj/Vertex.hpp"
#include "../scene/Mesh.hpp"

namespace av
{
	Mesh* TetrahedronBase::GenerateMesh()
	{
		std::vector<Vertex> vertices =
		{
			Vertex(Vector3f(0.5f, 0.0f, 0.0f), Vector4f(1.0f,0.0f,0.0f,1.0f)),
			Vertex(Vector3f(0.0f, 0.0f, 1.0f), Vector4f(0.0f,1.0f,0.0f,1.0f)),
			Vertex(Vector3f(1.0f, 0.0f, 1.0f), Vector4f(0.0f,0.0f,1.0f,1.0f)),
			Vertex(Vector3f(0.5f, 0.6f, 0.5f), Vector4f(0.3f,0.6f,0.6f,1.0f)),
		};

		std::vector<ushort> indices = { 0,1,2, 0,1,3, 1,2,3, 2,0,3 };

		PrimitiveType type = PrimitiveType::Triangles;

		Mesh* tri_mesh = new Mesh(
			"tetrahedron",
			vertices,
			indices,
			PrimitiveType::Triangles,
			nullptr,
			nullptr
		);

		return tri_mesh;
	}

	SceneEntityBuilder TetrahedronBase::Build()
	{
		SceneEntityBuilder sb;
		sb.WithMesh(*GetBaseMesh());
		sb.WithName("Tetrahedron #" + std::to_string(++counter));
		return sb;
	}

	std::string TetrahedronBase::GetPrimitiveName()
	{
		return "Tetrahedron";
	}
}