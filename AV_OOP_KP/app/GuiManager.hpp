#pragma once
#include <string>
#include <vector>
#include <functional>

namespace av
{
	class EditorApplication;
	class SceneEntity;

	class GuiManager
	{
		EditorApplication* App;

		bool ShowImGuiDemo = false;
		bool ShowPropertyWindow = false;
		bool ShowEntityListWindow = false;
		bool ShowMinimap = false;
		bool ShowToolbox = false;

		int PrimitiveIndex = 0;
		SceneEntity* DummyEntity;
		std::string Filename;
		std::vector<std::string> PrimitiveNames;

		void HandleCreate();
	public:
		GuiManager(EditorApplication* p_app);
		void DrawMainMenu();
		void DrawWindows();
		void DrawGui();

		void DrawPropertyWindow();
		void DrawEntityList();
		void DrawToolbox();
		void DrawMinimap();

		void ShowFileDialog(const std::function<void()> onConfirm);
		void ShowConfirmDialog(
			const std::string& description, 
			const std::function<void()> onConfirm
		);
	};

	class ModalWindow
	{
		std::string name;
		std::string text;

	};
}