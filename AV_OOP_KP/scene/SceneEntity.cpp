#include "SceneEntity.hpp"
#include "../globj/Program.hpp"

namespace av
{
	void SceneEntity::Draw()
	{
		UpdateUniforms();
		if (material != nullptr) material->UpdateUniforms();
		model->Draw();
	}

	void SceneEntity::UpdateTransform()
	{
		transform.UpdateTransform();
	}

	void SceneEntity::UpdateUniforms()
	{
		if (material == nullptr) return;
		material->GetProgram()->UpdateUniform("model_matrix",
			[&](GLuint u) { glUniformMatrix4fv(u, 1, false, transform.GetMatrix().data); });
		material->GetProgram()->UpdateUniform("base_color",
			[&](GLuint u) { glUniform4fv(u, 1, color.data()); });
	}
}