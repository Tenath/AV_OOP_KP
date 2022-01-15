#include <string>
#include <fstream>
#include <vector>

namespace av
{
	const float fPI = 3.14159265f;

	float DegToRad(float deg)
	{
		return deg * fPI / 180.0f;
	}

	std::string ReadTextFile(const std::string& filename)
	{
		std::string result;

		std::ifstream fs(filename, std::ios::in);

		std::string line;
		while (std::getline(fs, line))
		{
			result += line + "\n";
		}

		return result;
	}

	std::vector<std::string> ReadTextFileLines(const std::string& filename)
	{
		std::vector<std::string> result;

		std::ifstream fs(filename, std::ios::in);

		std::string line;
		while (std::getline(fs, line))
		{
			result.push_back(line);
		}

		return result;
	}

	std::vector<std::string> StringSplit(const std::string& str, char delimiter)
	{
		std::vector<std::string> result;

		size_t start = 0;
		size_t loc = 0;

		while ((loc = str.find(delimiter, start)) != std::string::npos)
		{
			result.push_back(str.substr(start, loc - start));
			start = loc + 1;
		}

		if (start < str.size()) result.push_back(str.substr(start));

		return result;
	}
}