#include "Material.hpp"
#include "../globj/Program.hpp"

namespace av
{
	void Material::UpdateUniforms()
	{
		if (program == nullptr) return;

		program->Bind();

		program->UpdateUniform("material.ambient",
			[&](GLuint u) { glUniform3fv(u, 1, Ambient.data()); });

		program->UpdateUniform("material.diffuse",
			[&](GLuint u) { glUniform3fv(u, 1, Diffuse.data()); });

		program->UpdateUniform("material.specular",
			[&](GLuint u) { glUniform3fv(u, 1, Specular.data()); });

		program->UpdateUniform("material.shininess",
			[&](GLuint u) { glUniform1fv(u, 1, &Shininess); });

		//program->Unbind();
	}

	Program* Material::GetProgram() { return program; }

	void Material::SetProgram(Program* prog)
	{
		program = prog;
	}
}