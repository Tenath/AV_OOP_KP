#pragma once
#pragma once
#include <string>
#include <vector>
#include <map>
#include "../math/av_vector_t.hpp"

namespace av
{
	float DegToRad(float deg);
	std::string ReadTextFile(const std::string& filename);
	std::vector<std::string> ReadTextFileLines(const std::string& filename);
	std::vector<std::string> StringSplit(const std::string& str, char delimiter = ' ');

	template <typename T> inline bool VectorContains(const std::vector<T>& vec, const T& value)
	{
		return std::find(vec.begin(), vec.end(), value) != vec.end();
	}

	template<typename K, typename V> inline bool MapContainsKey(const std::map<K, V>& m, const K& key)
	{
		return m.find(key) != m.end();
	}

	template<size_t D> Vector<float, D> UnpackVectorString(const std::string& str)
	{
		Vector<float, D> vec;

		std::string nobrackets = str.substr(1, str.size() - 2);
		if (nobrackets.length() == 0) return vec;
		auto components = StringSplit(nobrackets, ',');
		size_t count = components.size() < D ? components.size() : D;
		for (size_t i = 0; i < count; i++)
		{
			vec.at(i) = std::stof(components[i]);
		}

		return vec;
	}
}