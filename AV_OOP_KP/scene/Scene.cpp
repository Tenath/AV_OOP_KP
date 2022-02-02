#include "Scene.hpp"
#include "../globj/Program.hpp"

namespace av
{
	Scene::~Scene()
	{
		for (SceneEntity* obj : objects) delete obj;
	}

	void Scene::Draw()
	{
		cam.UpdateViewMatrix();
		perspective.UpdateProjection();

		// Basic, computationally expensive depth sorting
		std::vector<SceneEntity*> depth_sorted = objects;
		std::sort(depth_sorted.begin(), depth_sorted.end(),
			[this](SceneEntity* a, SceneEntity* b)
			{
				Vector4f apos = a->GetPosition();
				Vector4f bpos = b->GetPosition();
				Vector3f campos = cam.GetCameraPosVector();
				float a_dist = VectorDistance(campos, Vector3f(apos.X(), apos.Y(), apos.Z()));
				float b_dist = VectorDistance(campos, Vector3f(bpos.X(), bpos.Y(), bpos.Z()));

				return a_dist > b_dist;
			});

		for (SceneEntity* obj : depth_sorted) obj->Draw();
	}

	void Scene::AddObject(SceneEntity* obj)
	{
		if (obj == nullptr) throw std::exception("Attempting to add a null object to World");
		objects.push_back(obj);
	}

	void Scene::RemoveObject(SceneEntity* obj)
	{
		auto it = std::find(objects.begin(), objects.end(), obj);

		if (it != objects.end())
		{
			objects.erase(it);
		}
	}

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
}