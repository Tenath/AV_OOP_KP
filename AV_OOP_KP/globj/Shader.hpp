#pragma once
#include <string>
#include <vector>
#include <GL\gl3w.h>

namespace av
{
	enum class ShaderType : unsigned char
	{
		Vertex = 0,
		Geometry = 1,
		Fragment = 2,
		TessControl = 3,
		TessEval = 4,
		Undefined
	};

	struct ShaderFile
	{
		std::string filename;
		ShaderType type;

		ShaderFile(const std::string& src, ShaderType t) : filename(src), type(t) {}

		// Загружает CSV-файл со списком шейдеров в программе и формирует список
		static std::vector<ShaderFile> LoadShaderList(const std::string& filename);
	};

	class Shader
	{
	private:
		GLuint handle = 0;
		ShaderType type = ShaderType::Undefined;
		bool compiled = false;
		std::string source;
		std::string error;

		void Setup();
	public:
		Shader(const std::string& p_source, ShaderType p_type);
		Shader(Shader&& old) noexcept : handle(old.handle), type(old.type),
			compiled(old.compiled), source(old.source), error(old.error)
		{
			old.handle = 0;
		}

		void Compile();

		~Shader();

		GLuint GetHandle() { return handle; }
	};
}