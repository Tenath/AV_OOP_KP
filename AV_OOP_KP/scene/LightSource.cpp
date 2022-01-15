#include "LightSource.hpp"

namespace av
{
	void LightSource::UpdateUniforms(const std::string& varname)
	{
		/*Lab2App& app = (Lab2App&)Global::GetApp();
		app.UpdateUniform(varname + ".position",
			[&](GLuint u) { glUniform3fv(u, 1, position.data()); });
		app.UpdateUniform(varname + ".ambient",
			[&](GLuint u) { glUniform3fv(u, 1, ambient.data()); });
		app.UpdateUniform(varname + ".diffuse",
			[&](GLuint u) { glUniform3fv(u, 1, diffuse.data()); });
		app.UpdateUniform(varname + ".specular",
			[&](GLuint u) { glUniform3fv(u, 1, specular.data()); });

		app.UpdateUniform(varname + ".constant",
			[&](GLuint u) { glUniform1f(u, constant); });
		app.UpdateUniform(varname + ".linear",
			[&](GLuint u) { glUniform1f(u, linear); });
		app.UpdateUniform(varname + ".quadratic",
			[&](GLuint u) { glUniform1f(u, quadratic); });*/
	}

}