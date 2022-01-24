#pragma once
#include <string>
#include <vector>
#include <functional>

namespace av
{
	class EditorApplication;

	class GuiManager
	{
		EditorApplication* App;

		bool ShowImGuiDemo = false;
		bool ShowPropertyWindow = false;
		bool ShowEntityListWindow = false;
		bool ShowMinimap = false;
		bool ShowToolbox = false;

		std::string Filename;
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