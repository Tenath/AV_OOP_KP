#include <functional>
#include "Program.hpp"
#include "../utils/Utils.hpp"

namespace av
{
	Program::Program(const std::string& slist_file)
	{
		owner = true;
		handle = glCreateProgram();

		auto shaderlist = ShaderFile::LoadShaderList(slist_file);

		for (ShaderFile& s : shaderlist)
		{
			std::string src = ReadTextFile(s.filename);
			shaders.push_back(new Shader(src, s.type));
		}

		Build();
	}

	Program::Program()
	{
		handle = glCreateProgram();
	}

	void Program::AttachShader(Shader* shader)
	{
		if (owner)
		{
			throw std::logic_error("Program is in self-contained mode. \
				No new shaders can be attached manually");
		}

		if (VectorContains(shaders, shader)) throw std::exception(
			"Attempting to attach a duplicate shader to program");

		shaders.push_back(shader);
	}

	Program::~Program()
	{
		if (handle != 0) glDeleteProgram(handle);
		if (owner)
		{
			for (Shader* s : shaders)
			{
				delete s;
			}
		}
	}

	void Program::Bind() { glUseProgram(handle); }
	void Program::Unbind() { glUseProgram(0); }

	void Program::Build()
	{
		linked = false;
		for (Shader* s : shaders) glAttachShader(handle, s->GetHandle());

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
		for (Shader* s : shaders) glDetachShader(handle, s->GetHandle());
	}

	void Program::BuildUniformList()
	{
		if (!IsBuilt()) throw std::exception("Building a uniform list for a program that's not linked!");
		uniforms.clear();

		GLint uniform_count = 0;
		glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &uniform_count);

		GLint buf_size = 0;
		glGetProgramiv(handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &buf_size);

		std::vector<GLchar> uniform_name(buf_size + 1, '\0');

		for (GLint i = 0; i < uniform_count; i++)
		{
			Uniform uf;
			uf.Index = i;
			glGetActiveUniform(handle, i, buf_size, nullptr, &uf.Size, &uf.Type, uniform_name.data());
			uf.Name.assign(uniform_name.data());
			uniforms[uf.Name] = uf;
		}
	}

	bool Program::UpdateUniform(
		const std::string& uniform, 
		const std::function<void(GLuint)>& func
	)
	{
		if (uniforms.find(uniform) == uniforms.end()) return false;

		func(uniforms[uniform].Index);
		return true;
	}
}