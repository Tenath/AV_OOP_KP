#include "Scene.hpp"
#include "SceneEntity.hpp"
#include "../globj/Program.hpp"

namespace av
{
	void SceneEntity::SetScene(Scene* sc)
	{
		if (scene != nullptr) scene->RemoveObject(this);
		scene = sc;
		if (sc != nullptr) scene->AddObject(this);
	}

	void SceneEntity::Draw()
	{
		if (material != nullptr)
		{
			material->GetProgram()->Bind();
			UpdateUniforms();
			material->UpdateUniforms();
		}
		model->Draw();
	}

	void SceneEntity::UpdateTransform()
	{
		transform.UpdateTransform();
	}

	void SceneEntity::UpdateUniforms()
	{
		if (material == nullptr) return;
		material->GetProgram()->Bind();
		material->GetProgram()->UpdateUniform("model_matrix",
			[&](GLuint u) { glUniformMatrix4fv(u, 1, false, transform.GetMatrix().data); });
		material->GetProgram()->UpdateUniform("base_color",
			[&](GLuint u) { glUniform4fv(u, 1, color.data()); });

		if (scene != nullptr)
		{
			scene->UpdateView(*material->GetProgram());
			scene->UpdateProjection(*material->GetProgram());
		}
	}
}