#include "Scene.hpp"
#include "../globj/Program.hpp"

namespace av
{
	void Scene::RecomputeView()
	{
		cam.UpdateViewMatrix();
	}

	void Scene::RecomputeProjection()
	{
		perspective.UpdateProjection();
	}

	void Scene::UpdateView(Program& prog)
	{
		Vector3f campos = cam.GetCameraPosVector();

		prog.UpdateUniform("camera_pos", 
			[&](GLuint u) { glUniform3fv(u, 1, cam.GetCameraPosVector().data()); });

		prog.UpdateUniform("view_matrix",
			[&](GLuint u) { glUniformMatrix4fv(u, 1, false, cam.GetViewMatrix().data); });
	}

	void Scene::UpdateProjection(Program& prog)
	{
		prog.UpdateUniform("projection_matrix", 
			[&](GLuint u) { glUniformMatrix4fv(u, 1, false, perspective.GetProjection().data); });
	}

	void Scene::UpdateLightSourceUniforms()
	{
	}
}