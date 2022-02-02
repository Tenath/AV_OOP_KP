#pragma once
#include <string>
#include <vector>
#include <functional>
#include <ImGuiFileBrowser.h>

namespace av
{
	class EditorApplication;
	class SceneEntity;

	class GuiManager
	{
		EditorApplication* App;

		bool ShowImGuiDemo = false;
		bool ShowEntityListWindow = false;
		bool ShowToolbox = false;

		bool ShowOpen = false;
		bool ShowSave = false;

		int PrimitiveIndex = 0;
		SceneEntity* DummyEntity;
		std::string Filename;
		std::vector<std::string> PrimitiveNames;

		imgui_addons::ImGuiFileBrowser file_dialog;

		void HandleCreate();
		void HandleDelete(SceneEntity* entity);
	public:
		GuiManager(EditorApplication* p_app);
		void DrawMainMenu();
		void DrawWindows();
		void DrawGui();
		void DrawFileDialogs();

		void DrawEntityList();
		void DrawToolbox();
	};

	class ModalWindow
	{
		std::string name;
		std::string text;

	};
}