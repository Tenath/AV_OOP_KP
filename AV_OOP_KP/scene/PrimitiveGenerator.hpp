#pragma once
#include <string>
#include "../globals.hpp"
#include "../globj/VertexArray.hpp"
#include "../globj/Vertex.hpp"
#include "../primitives/PrimitiveBase.hpp"

namespace av
{
	class PrimitiveGenerator
	{
	private:
		std::map<std::string, PrimitiveBase*> data;
		VertexArray<Vertex, USHORT>* va;
	public:
		bool RegisterBase(PrimitiveBase* base);
		PrimitiveBase* RequestPrimitive(const std::string& name);
		VertexArray<Vertex, USHORT>* GetVertexArray();
		std::map<std::string, PrimitiveBase*>& GetData();
		std::vector<std::string> GetPrimitiveNames();

		PrimitiveGenerator();
		~PrimitiveGenerator();
	};
}