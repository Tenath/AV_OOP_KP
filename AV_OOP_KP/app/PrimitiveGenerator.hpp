#pragma once
#include <string>
#include "../globals.hpp"
#include "../globj/VertexArray.hpp"
#include "../globj/Vertex.hpp"
#include "../primitives/PrimitiveFactory.hpp"

namespace av
{
	class PrimitiveGenerator
	{
	private:
		std::map<std::string, PrimitiveFactory*> data;
		VertexArray<Vertex, USHORT>* va;
	public:
		PrimitiveFactory& operator[](const std::string& name);
		bool RegisterFactory(PrimitiveFactory* base);
		PrimitiveFactory* RequestFactory(const std::string& name);
		VertexArray<Vertex, USHORT>* GetVertexArray();
		std::map<std::string, PrimitiveFactory*>& GetData();
		std::vector<std::string> GetPrimitiveNames();

		PrimitiveGenerator();
		~PrimitiveGenerator();
	};
}