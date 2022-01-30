#pragma once
#include <string>
#include "../scene/Scene.hpp"

namespace av
{
	class Scene;

	class SceneManager
	{
	private:
		std::string filename = "";
		Scene* current_scene;

		void Discard();
	public:
		void New();
		void Open(const std::string& filename) {}
		void Save() {}
		void SaveAs(const std::string& filename) {}
		void Close() {};

		SceneManager()
		{
			New();
		}

		std::string GetActiveFile() { return filename; }
		Scene* GetScene() { return current_scene; }
		void Draw() { if (current_scene != nullptr) current_scene->Draw(); }
	};
}