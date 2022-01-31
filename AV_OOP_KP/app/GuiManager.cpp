#include "GuiManager.hpp"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "EditorApplication.hpp"

namespace av
{
	GuiManager::GuiManager(EditorApplication* p_app) 
		: App(p_app) {}

	void GuiManager::DrawGui()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(App->GetWindow());
		ImGui::NewFrame();

		DrawWindows();
		DrawMainMenu();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void GuiManager::DrawWindows()
	{
		if (ShowImGuiDemo) ImGui::ShowDemoWindow(&ShowImGuiDemo);
		if (ShowEntityListWindow) DrawEntityList();
		if (ShowPropertyWindow) DrawPropertyWindow();
		if (ShowMinimap) DrawMinimap();
	}

	void GuiManager::DrawMainMenu()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N")) 
				{ 
					ShowConfirmDialog("Create a new scene without saving changes?",
						[this]() { App->GetSceneManager().New(); });
				}
				if (ImGui::MenuItem("Open", "Ctrl+O")) 
				{ 
					ShowFileDialog([this]() {App->GetSceneManager().Open(Filename); });
				}
				if (ImGui::MenuItem("Save", "Ctrl+S")) 
				{ 
					if (App->GetSceneManager().GetActiveFile().length() > 0)
						App->GetSceneManager().Save();
					else ShowFileDialog([this]() { App->GetSceneManager().Save(); });
				}
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					ShowFileDialog([this]() { App->GetSceneManager().SaveAs(Filename); });
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Close", "Ctrl+W")) { App->GetSceneManager().Close(); }
				if (ImGui::MenuItem("Exit")) { App->Exit(); }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Entity List")) ShowEntityListWindow = true;
				if (ImGui::MenuItem("Property Window")) ShowPropertyWindow = true;
				if (ImGui::MenuItem("Minimap")) ShowMinimap = true;
				ImGui::Separator();
				if (ImGui::MenuItem("ImGui demo window")) { ShowImGuiDemo = true; }

				ImGui::EndMenu();
			}
			/*if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("ImGui demo window"))
				{
					ShowImGuiDemo = true;
				}
				ImGui::EndMenu();
			}*/
		}
		ImGui::EndMainMenuBar();
	}

	void GuiManager::ShowFileDialog(const std::function<void()> onConfirm)
	{

	}

	void GuiManager::ShowConfirmDialog(
		const std::string& description, 
		const std::function<void()> onConfirm)
	{
		ImGui::OpenPopup("ConfirmDialog");

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if(ImGui::BeginPopupModal("ConfirmDialog", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text(description.c_str());
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }

			ImGui::EndPopup();
		}
		
	}

	void GuiManager::DrawPropertyWindow()
	{
		ImGui::Begin("Properties", &ShowPropertyWindow);

		ImGui::End();
	}

	void GuiManager::DrawEntityList()
	{
		ImGui::Begin("Entity List", &ShowEntityListWindow);
		
		//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		if (ImGui::TreeNode("Scene Entities"))
		{
			auto objects = App->GetSceneManager().GetScene()->GetObjects();
			const char* fmt = "%d: %s";

			for (size_t i = 0; i < objects.size(); i++)
			{
				std::string name = "entity_" + std::to_string(i);

				if (ImGui::TreeNode(name.c_str(), fmt, i + 1, objects[i]->GetName().c_str()))
				{
					ImGui::TreeNode(("entity_prop_pos_" + std::to_string(i)).c_str(), "Position: ");
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100.0f);
					if (ImGui::InputFloat("x", &objects[i]->GetTransform().GetPosition().X(), 0.01f, 0.1f))
					{
						objects[i]->UpdateTransform();
					}
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100.0f);
					if (ImGui::InputFloat("y", &objects[i]->GetTransform().GetPosition().Y(), 0.01f, 0.1f))
					{
						objects[i]->UpdateTransform();
					}
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100.0f);
					if (ImGui::InputFloat("z", &objects[i]->GetTransform().GetPosition().Z(), 0.01f, 0.1f))
					{
						objects[i]->UpdateTransform();
					}

					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

		ImGui::End();
	}

	void GuiManager::DrawToolbox()
	{
		ImGui::Begin("Toolbox", &ShowToolbox);

		ImGui::End();
	}

	void GuiManager::DrawMinimap()
	{
		ImGui::SetWindowPos("Minimap", ImVec2(0.8f, 0.2f), ImGuiCond_Once);
		ImGui::SetWindowSize("Minimap", ImVec2(100.f, 100.f), ImGuiCond_Once);
		ImGui::Begin("Minimap", &ShowMinimap);

		ImGui::End();
	}
}