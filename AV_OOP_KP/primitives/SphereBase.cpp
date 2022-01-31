#include <cmath>
#include "SphereBase.hpp"
#include "../globj/Vertex.hpp"
#include "../scene/Mesh.hpp"

namespace av
{
	Mesh* SphereBase::GenerateMesh()
	{
		std::vector<Vertex> vertices;

		const int LATITUDE_COUNT = 20;
		const int LONGITUDE_COUNT = 20;

		std::vector<float> zlevels;

		// North pole
		vertices.push_back(Vertex(Vector3f(0.0f, 1.0f, 0.0f), Vector4f(0.0f,0.0f,0.5f,1.0f)));

		for (int lat = 1; lat < LATITUDE_COUNT; ++lat)
		{
			zlevels.push_back(lat / (float)LATITUDE_COUNT);
			for (int lon = 0; lon < LONGITUDE_COUNT; ++lon)
			{
				Vector3f pos(
					sin(PI * lat / (float)LATITUDE_COUNT) * cos(2.0f * PI * lon / (float)LONGITUDE_COUNT),
					cos(PI * lat / (float)LATITUDE_COUNT),
					sin(PI * lat / (float)LATITUDE_COUNT) * sin(2.0f * PI * lon / (float)LONGITUDE_COUNT)
				);

				vertices.push_back(Vertex(pos, Vector4f(0.0f, lat / (float)LATITUDE_COUNT, 0.5f, 1.0f)));
			}
		}

		// South pole
		vertices.push_back(Vertex(Vector3f(0.0f, -1.0f, 0.0f), Vector4f(0.0f, 0.0f, 0.5f,1.0f)));

		std::vector<ushort> indices;

		// Top
		for (int i = 0; i < LONGITUDE_COUNT; ++i)
		{
			ushort i2 = (i + 1) % (LONGITUDE_COUNT + 1);
			ushort i3 = (i + 2) % (LONGITUDE_COUNT + 1);
			if (i2 == 0) i2 += 1;
			else if (i3 == 0) i3 += 1;

			indices.push_back(0);
			//indices.push_back((i + 1)%(LONGITUDE_COUNT+1));
			//indices.push_back((i + 2)%(LONGITUDE_COUNT+1));
			indices.push_back(i2);
			indices.push_back(i3);
		}

		// z-levels
		for (int lat = 0; lat < LATITUDE_COUNT-2; ++lat)
		{
			for (int lon = 0; lon < LONGITUDE_COUNT; ++lon)
			{
				indices.push_back(1 + lat * LATITUDE_COUNT + lon);
				indices.push_back(1 + lat * LATITUDE_COUNT + (lon +1));
				indices.push_back(1 + (lat+1) * LATITUDE_COUNT + lon);

				indices.push_back(1 + (lat + 1) * LATITUDE_COUNT + lon);
				indices.push_back(1 + lat * LATITUDE_COUNT + lon + 1);
				indices.push_back(1 + (lat + 1) * LATITUDE_COUNT + lon+1);
			}
		}

		// Bottom
		for (int i = 0; i < LONGITUDE_COUNT; ++i)
		{
			ushort i2 = (i + 1) % (LONGITUDE_COUNT + 1);
			ushort i3 = (i + 2) % (LONGITUDE_COUNT + 1);
			if (i2 == 0) i2 += 1;
			else if (i3 == 0) i3 += 1;

			indices.push_back(vertices.size() - 1);
			//indices.push_back((i + 1)%(LONGITUDE_COUNT+1));
			//indices.push_back((i + 2)%(LONGITUDE_COUNT+1));
			indices.push_back(vertices.size() - 1 - i2);
			indices.push_back(vertices.size() - 1 - i3);
		}

		//indices.push_back(0);
		//indices.push_back((LONGITUDE_COUNT+1) % LONGITUDE_COUNT)

		PrimitiveType type = PrimitiveType::Triangles;

		Mesh* tri_mesh = new Mesh(
			"sphere",
			vertices,
			indices,
			PrimitiveType::Triangles,
			nullptr,
			nullptr
		);

		return tri_mesh;
	}

	SceneEntityBuilder SphereBase::Build()
	{
		SceneEntityBuilder sb;
		sb.WithMesh(*GetBaseMesh());
		sb.WithName("Sphere #" + std::to_string(++counter));
		return sb;
	}
}