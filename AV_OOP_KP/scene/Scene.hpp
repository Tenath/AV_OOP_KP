// (c) 2020-2021 Andrei Veeremaa 
#pragma once
#include <vector>
#include "../math/av_camera.hpp"
#include "SceneEntity.hpp"
#include "Perspective.hpp"

namespace av
{
	class Scene
	{
		std::vector<SceneEntity*> objects;
		Camera cam;
		Perspective perspective;

	public:
		~Scene();

		void Draw();

		void AddObject(SceneEntity* obj);
		void RemoveObject(SceneEntity* obj);

		void RecomputeView();
		void RecomputeProjection();

		void UpdateView(Program& prog);
		void UpdateProjection(Program& prog);

		Camera& GetCamera() noexcept { return cam; }
		Perspective& GetPerspective() noexcept { return perspective; }

		std::vector<SceneEntity*>& GetObjects() { return objects; }
	};
}