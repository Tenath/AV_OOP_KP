#include "GLObject.hpp"
#include "Utilities.hpp"
using namespace av;

ShaderType ParseShaderType(const std::string& str)
{
	ShaderType result = ShaderType::Undefined;

	if (str == "Vertex") result = ShaderType::Vertex;
	else if (str == "Fragment") result = ShaderType::Fragment;
	else if (str == "Geometry") result = ShaderType::Geometry;
	else if (str == "TessControl") result = ShaderType::TessControl;
	else if (str == "TessEval") result = ShaderType::TessEval;

	return result;
}

GLenum ShaderTypeToGLenum(ShaderType t)
{
	GLenum type_enum = 0;
	
	switch (t)
	{
	case ShaderType::Vertex: type_enum = GL_VERTEX_SHADER; break;
	case ShaderType::Fragment: type_enum = GL_FRAGMENT_SHADER; break;
	case ShaderType::Geometry: type_enum = GL_GEOMETRY_SHADER; break;
	case ShaderType::TessControl: type_enum = GL_TESS_CONTROL_SHADER; break;
	case ShaderType::TessEval: type_enum = GL_TESS_EVALUATION_SHADER; break;
	}

	return type_enum;
}

std::vector<ShaderFile> ShaderFile::LoadShaderList(const std::string& filename)
{
	std::vector<ShaderFile> result;

	std::vector<std::string> lines = ReadTextFileLines(filename);

	for (std::string& line : lines)
	{
		auto parts = StringSplit(line, ',');
		ShaderFile sf(parts[0], ParseShaderType(parts[1]));
		result.push_back(sf);
	}

	return result;
}

Program::Program(const std::string& slist_file)
{
	handle = glCreateProgram();

	auto shaderlist = ShaderFile::LoadShaderList(slist_file);

	for (ShaderFile& s : shaderlist)
	{
		std::string src = ReadTextFile(s.filename);
		shaders.emplace_back(Shader(src, s.type));
	}

	Build();
}

Program::~Program()
{
	if (handle != 0) glDeleteProgram(handle);
}

void Program::Bind() { glUseProgram(handle); }
void Program::Unbind() { glUseProgram(0); }

Shader::Shader(const std::string& p_source, ShaderType p_type) : source(p_source), type(p_type)
{
	Setup();
	Compile();
}

Shader::~Shader()
{
	if (handle != 0) glDeleteShader(handle);
}

void Shader::Setup()
{
	handle = glCreateShader(ShaderTypeToGLenum(type));
}

void Shader::Compile()
{
	compiled = false;
	const char* srcptr = source.c_str();

	glShaderSource(handle, 1, &srcptr, 0);
	glCompileShader(handle);

	GLint compile_result = GL_FALSE;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &compile_result);

	if (compile_result == GL_FALSE)
	{
		GLint infolog_length = 0;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &infolog_length);

		std::vector<GLchar> infolog(infolog_length + 1, '\0');

		glGetShaderInfoLog(handle, infolog_length, 0, infolog.data());

		error = infolog.data();
		infolog.clear();
		
		std::string exception_msg = "Shader compile error: " + error;
		throw std::logic_error(exception_msg);
	}

	compiled = true;
}

void Program::Build()
{
	linked = false;
	for (Shader& s : shaders) glAttachShader(handle, s.GetHandle());

	glLinkProgram(handle);

	GLint link_result = GL_FALSE;

	glGetProgramiv(handle, GL_LINK_STATUS, &link_result);

	if (link_result == GL_FALSE)
	{
		GLint infolog_length = 0;
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &infolog_length);

		std::vector<GLchar> infolog(infolog_length + 1, '\0');
		glGetProgramInfoLog(handle, infolog_length, 0, infolog.data());
		error = infolog.data();
	}

	linked = true;
	for (Shader& s : shaders) glDetachShader(handle, s.GetHandle());
}