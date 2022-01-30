#pragma once
#include <string>

namespace av
{
	class PrimitiveRepository
	{
		virtual Model* RequestModel(std::string) = 0;
	};
}