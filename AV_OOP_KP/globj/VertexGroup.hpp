#pragma once
#include <vector>
#include <string>

namespace av
{
	template <typename VertexT>
	class VertexGroup
	{
		std::string name;
		std::vector<VertexT> vertices;

	public:
		VertexGroup(
			const std::string& p_name,
			const std::initializer_list<VertexT>& lst
		) : name(p_name), vertices(lst) {}

		VertexGroup(
			const std::string& p_name,
			const std::vector<VertexT>& lst
		) : name(p_name), vertices(lst) {}

		std::string GetName() { return name; }
		void SetName(const std::string& new_name) { name = new_name; }

		std::vector<VertexT>& GetVertices() { return vertices; }
	};

	enum class PrimitiveType : unsigned char
	{
		Points = 0,
		Lines = 1,
		Triangles = 2,
		TriFan = 3,
		TriStrip = 4
	};

	template <typename IndexT>
	class IndexGroup
	{
		std::string name;
		std::vector<IndexT> indices;
		PrimitiveType type = PrimitiveType::Triangles;

	public:
		IndexGroup(
			const std::string& p_name,
			const std::initializer_list<IndexT>& lst
		) : name(p_name), indices(lst) {}

		IndexGroup(
			const std::string& p_name,
			const std::vector<IndexT>& lst,
			PrimitiveType p_type
		) : name(p_name), indices(lst), type(p_type) {}

		std::vector<IndexT>& GetIndices() { return indices; }
		PrimitiveType GetType() { return type; }
	};
}