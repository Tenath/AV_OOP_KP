#pragma once
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <GL\gl3w.h>
#include "BindableObject.hpp"
#include "Shader.hpp"

namespace av
{
	struct Uniform
	{
		GLuint Index = 0;
		std::string Name;
		GLint Size = 0;
		GLenum Type = GL_NONE;
	};

	class Program : BindableGLObject
	{
	private:
		GLuint handle = 0;
		std::vector<Shader*> shaders;
		std::string error;
		bool linked = false;
		bool owner = false;

		std::map<std::string, Uniform> uniforms;

	public:
		void Build();
		Program(const std::string& slist_file);
		Program();
		~Program();

		void AttachShader(Shader* shader);

		void Bind() override;
		void Unbind() override;

		GLuint GetHandle() override { return handle; }
		bool IsBuilt() { return linked; }
		std::string GetError() { return error; }

		void BuildUniformList();

		bool UpdateUniform(const std::string& uniform, const std::function<void(GLuint)>& func);

		Uniform GetUniform(const std::string& name) { return uniforms[name]; }
	};
}