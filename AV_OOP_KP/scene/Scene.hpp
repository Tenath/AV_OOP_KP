// (c) 2020-2021 Andrei Veeremaa 
#pragma once
#include <vector>
#include "../math/av_camera.hpp"
#include "SceneEntity.hpp"
#include "LightSource.hpp"
#include "ObjectGroup.hpp"
#include "Perspective.hpp"

namespace av
{
	class Scene
	{
		std::vector<SceneEntity*> objects;
		std::vector<LightSource*> sources;
		std::vector<ObjectGroup*> obj_groups;
		Camera cam;
		Perspective perspective;

	public:
		~Scene()
		{
			for (SceneEntity* obj : objects) delete obj;
			for (LightSource* ls : sources) delete ls;
			for (ObjectGroup* objgrp : obj_groups) delete objgrp;
		}

		void Draw()
		{
			cam.UpdateViewMatrix();
			perspective.UpdateProjection();
			UpdateLightSourceUniforms();

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

		void AddObject(SceneEntity* obj)
		{
			if (obj == nullptr) throw std::exception("Attempting to add a null object to World");
			objects.push_back(obj);
		}

		void RemoveObject(SceneEntity* obj)
		{
			auto it = std::find(objects.begin(), objects.end(), obj);

			if(it!=objects.end())
			{
				objects.erase(it);
			}
		}

		void AddLightSource(LightSource* ls)
		{
			if (ls == nullptr) throw std::exception("Attempting to add a null light source to the World");
			ls->SetIndex(sources.size());
			sources.push_back(ls);
		}

		void AddObjectGroup(ObjectGroup* objgr)
		{
			if (objgr == nullptr) throw std::exception("Attempting to add a null object group to World");
			obj_groups.push_back(objgr);
		}

		void RecomputeView();
		void RecomputeProjection();

		void UpdateLightSourceUniforms();
		void UpdateView(Program& prog);
		void UpdateProjection(Program& prog);

		Camera& GetCamera() noexcept { return cam; }
		Perspective& GetPerspective() noexcept { return perspective; }

		LightSource* GetLightSource(size_t index) { return index < sources.size() ? sources[index] : nullptr; }
		size_t GetLightSourceCount() { return sources.size(); }

		ObjectGroup* GetObjectGroup(size_t index) { return index < obj_groups.size() ? obj_groups[index] : nullptr; }

		std::vector<SceneEntity*>& GetObjects() { return objects; }
		// TODO: Поиск по названию группы объектов
	};
}