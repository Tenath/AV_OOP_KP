#include "PrimitiveGenerator.hpp"
#include "../utils/Utils.hpp"

namespace av
{
	PrimitiveFactory& PrimitiveGenerator::operator[](const std::string& name)
	{
		PrimitiveFactory* fac = RequestFactory(name);
		if (fac == nullptr) 
			throw std::invalid_argument("Primitive Factory does not exist");
		return *fac;
	}
	bool PrimitiveGenerator::RegisterFactory(PrimitiveFactory* base)
	{
		std::string name = base->GetPrimitiveName();
		if (MapContainsKey(data,name)) return false;
		data[name] = base;
		return true;
	}

	VertexArray<Vertex, USHORT>* PrimitiveGenerator::GetVertexArray()
	{
		return va;
	}

	std::map<std::string, PrimitiveFactory*>& PrimitiveGenerator::GetData()
	{
		return data;
	}

	std::vector<std::string> PrimitiveGenerator::GetPrimitiveNames()
	{
		std::vector<std::string> result;

		for (auto entry : data)
		{
			result.push_back(entry.first);
		}

		return result;
	}

	PrimitiveFactory* PrimitiveGenerator::RequestFactory(const std::string& name)
	{
		auto it = data.find(name);
		return (it != data.end()) ? it->second : nullptr;
	}

	PrimitiveGenerator::PrimitiveGenerator()
	{
		va = new VertexArray<Vertex, USHORT>();
	}

	PrimitiveGenerator::~PrimitiveGenerator()
	{
		for (auto entry : data)
		{
			delete entry.second;
		}

		delete va;
	}
}