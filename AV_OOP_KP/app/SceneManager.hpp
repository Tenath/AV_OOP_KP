#pragma once
#include <string>
#include "../scene/Scene.hpp"

namespace av
{
	class Scene;
	class EditorApplication;

	class SceneManager
	{
	private:
		std::string filename = "";
		Scene* current_scene;
		EditorApplication* app;

		void SaveScene(const std::string& filename, Scene* scene);
		Scene* LoadScene(const std::string& filename);
		void Discard();
	public:
		void New();
		void Open(const std::string& filename);
		void Save();
		void SaveAs(const std::string& filename);

		void Close();

		SceneManager(EditorApplication* p_app);

		std::string GetActiveFile();
		Scene* GetScene();
		void Draw();
	};
}