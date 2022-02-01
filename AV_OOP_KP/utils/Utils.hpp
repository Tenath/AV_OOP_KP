#pragma once
#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>
#include "../math/av_vector_t.hpp"

namespace av
{
	float DegToRad(float deg);
	std::string ReadTextFile(const std::string& filename);
	std::vector<std::string> ReadTextFileLines(const std::string& filename);
	// Запись из вектора строчек в файл
	void WriteTextFileLines(const std::string& filename, const std::vector<std::string>& lines);
	std::vector<std::string> StringSplit(const std::string& str, char delimiter = ' ');

	template <typename T> inline size_t VectorFindIndex(const std::vector<T>& vec, const T& value)
	{
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (vec[i] == value) return i;
		}

		return -1;
	}

	template <typename T> inline bool VectorContains(const std::vector<T>& vec, const T& value)
	{
		return std::find(vec.begin(), vec.end(), value) != vec.end();
	}

	template <typename T> inline bool VectorContainsPredicate(const std::vector<T>& vec, std::function<bool(const T&)> predicate)
	{
		bool result = false;

		for (const T& val : vec)
		{
			result = predicate(val);
			if (result) return true;
		}

		return result;
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

	template<size_t D> std::string PackVectorString(const Vector<float, D>& vec)
	{
		std::string result = "{";

		for (size_t i = 0; i < vec.size()-1; i++)
		{
			result += std::to_string(vec.at(i)) + ",";
		}

		result += std::to_string(vec.at(vec.size() - 1)) + "}";

		return result;
	}
}