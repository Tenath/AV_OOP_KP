#include "DiamondBase.hpp"
#include "../globj/Vertex.hpp"
#include "../scene/Mesh.hpp"

namespace av
{
	Mesh* DiamondBase::GenerateMesh()
	{
		std::vector<Vertex> vertices;

		// Координата y для кончика
		const float tip_level = -0.3f;
		const float mid_level = 0.0f;
		const float top_level = 0.1f;

		const float outer_r = 0.2f;
		const float inner_r = 0.15f;

		vertices.clear();

		// Кончик "алмаза"
		vertices.push_back(Vertex(Vector3f(0.0f, tip_level, 0.0f), Vector4f(0.0f, 0.7f, 0.7f, 1.0f)));

		// Вертексы среднего уровня высоты
		float x = 0.0f, y = 0.0f, z = mid_level;
		for (int i = 0; i <= 5; i++)
		{
			x = sin((i / 3.0f) * PI) * outer_r;
			y = cos((i / 3.0f) * PI) * outer_r;

			vertices.push_back(Vertex(Vector3f(x, y, z),Vector4f(0.0f,0.7f,0.7f,1.0f)));
		}

		// "Крышка" алмаза
		z = top_level;
		for (int i = 0; i <= 5; i++)
		{
			x = sin((i / 3.0f) * PI) * inner_r;
			y = cos((i / 3.0f) * PI) * inner_r;

			vertices.push_back(Vertex(Vector3f(x, y, z), Vector4f(0.0f, 0.7f, 0.7f, 1.0f)));
		}

		vertices.push_back(Vertex(Vector3f(0.0f, top_level, 0.0f), Vector4f(0.0f, 0.7f, 0.7f, 1.0f)));

		std::vector<USHORT> indices =
		{
			1,0,2,3,0,4,5,0,6,1,65535,
			//0,1,2,3,4,5,6,1, // tri-fan #1
			//13,12,11,10,9,8,7,12, // tri-fan #2
			12,13,11,10,13,9,8,13,7,12,65535,
			1,7,2,8,3,9,4,10,5,11,6,12,1,7 // tri-strip
		};

		PrimitiveType type = PrimitiveType::TriStrip;

		Mesh* tri_mesh = new Mesh(
			"triangle",
			vertices,
			indices,
			PrimitiveType::Triangles,
			nullptr,
			nullptr
		);

		return tri_mesh;
	}

	SceneEntityBuilder DiamondBase::Build()
	{
		SceneEntityBuilder sb;
		sb.WithMesh(*GetBaseMesh());
		return sb;
	}
}