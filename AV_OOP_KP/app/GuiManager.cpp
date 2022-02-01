#include "GuiManager.hpp"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "imgui_stdlib.h"
#include "EditorApplication.hpp"
#include "PrimitiveGenerator.hpp"
#include <ImGuiFileBrowser.h>

namespace av
{
	GuiManager::GuiManager(EditorApplication* p_app) 
		: App(p_app), DummyEntity(new SceneEntity()) {}

	void GuiManager::DrawGui()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(App->GetWindow());
		ImGui::NewFrame();

		DrawWindows();
		DrawMainMenu();
		DrawFileDialogs();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void GuiManager::DrawWindows()
	{
		if (ShowImGuiDemo) ImGui::ShowDemoWindow(&ShowImGuiDemo);
		if (ShowEntityListWindow) DrawEntityList();
		if (ShowPropertyWindow) DrawPropertyWindow();
		if (ShowToolbox) DrawToolbox();
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
					/*ShowConfirmDialog("Create a new scene without saving changes?",
						[this]() { App->GetSceneManager().New(); });*/
					App->GetSceneManager().New();
				}
				if (ImGui::MenuItem("Open", "Ctrl+O")) 
				{ 
					ShowOpen = true;
					//ShowFileDialog([this]() {App->GetSceneManager().Open(Filename); });
				}
				if (ImGui::MenuItem("Save", "Ctrl+S")) 
				{ 
					if (Filename.size() == 0)
					{
						ShowSave=true;
					}
					else App->GetSceneManager().SaveAs(Filename);
					/*if (App->GetSceneManager().GetActiveFile().length() > 0)
						App->GetSceneManager().Save();
					else ShowFileDialog([this]() { App->GetSceneManager().Save(); });*/
				}
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					ShowSave = true;
					//ShowFileDialog([this]() { App->GetSceneManager().SaveAs(Filename); });
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Close", "Ctrl+W")) { App->GetSceneManager().Close(); }
				if (ImGui::MenuItem("Exit")) { App->Exit(); }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Entity List")) ShowEntityListWindow = true;
				if (ImGui::MenuItem("Toolbox")) ShowToolbox = true;
				//if (ImGui::MenuItem("Property Window")) ShowPropertyWindow = true;
				//if (ImGui::MenuItem("Minimap")) ShowMinimap = true;

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Test"))
			{
				if (ImGui::MenuItem("Setup Test Scene")) { App->SetupTestScene(); }
				if (ImGui::MenuItem("ImGui demo window")) { ShowImGuiDemo = true; }
			
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();
	}

	void GuiManager::DrawFileDialogs()
	{
		if(ShowOpen)
			ImGui::OpenPopup("Open File");

		if(ShowSave)
			ImGui::OpenPopup("Save File");

		if (file_dialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310), ".scn"))
		{
			Filename = file_dialog.selected_path;
			App->GetSceneManager().Open(Filename);
			ShowOpen = false;
		}
		if (file_dialog.showFileDialog("Save File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(700, 310), ".scn"))
		{
			Filename = file_dialog.selected_path;
			App->GetSceneManager().SaveAs(Filename);
			ShowSave = false;
		}
	}
	/*void GuiManager::ShowFileDialog(const std::function<void()> onConfirm)
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
		
	}*/

	void GuiManager::DrawPropertyWindow()
	{
		ImGui::Begin("Properties", &ShowPropertyWindow);

		ImGui::End();
	}

	void GuiManager::DrawEntityList()
	{
		SceneEntity* deleting = nullptr;

		ImGui::Begin("Entity List", &ShowEntityListWindow);
		
		//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		if (ImGui::TreeNode("Scene Entities"))
		{
			auto objects = App->GetSceneManager().GetScene()->GetObjects();
			const char* fmt = "%d: %s";

			for (size_t i = 0; i < objects.size(); i++)
			{
				std::string name = "entity_" + std::to_string(i);
				ImGui::PushID(objects[i]);
				if (ImGui::TreeNode(name.c_str(), fmt, i + 1, objects[i]->GetName().c_str()))
				{
					if(ImGui::TreeNode("entity_name", "Name:")) { ImGui::TreePop(); }
					ImGui::SameLine();
					ImGui::SetNextItemWidth(200.0f);
					ImGui::InputText("", &objects[i]->EditName());
					if(ImGui::TreeNode("entity_prop_pos", "Position: ")) { ImGui::TreePop(); }
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

					if(ImGui::TreeNode("entity_prop_rot", "Rotation: ")) { ImGui::TreePop(); }
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100.0f);
					if (ImGui::SliderAngle("X", &objects[i]->GetTransform().GetRotation().X()))
					{
						objects[i]->GetTransform().UpdateTransform();
					}

					ImGui::SameLine();
					ImGui::SetNextItemWidth(100.0f);
					if (ImGui::SliderAngle("Y", &objects[i]->GetTransform().GetRotation().Y()))
					{
						objects[i]->GetTransform().UpdateTransform();
					}

					ImGui::SameLine();
					ImGui::SetNextItemWidth(100.0f);
					if (ImGui::SliderAngle("Z", &objects[i]->GetTransform().GetRotation().Z()))
					{
						objects[i]->GetTransform().UpdateTransform();
					}

					if(ImGui::TreeNode("entity_scale", "")) { ImGui::TreePop(); }
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100.0f);
					if(ImGui::DragFloat("Scale", &objects[i]->GetTransform().GetScale(), 0.005f, 0.1f, 10.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
					{
						objects[i]->GetTransform().UpdateTransform();
					}
					
					ImGui::SetNextItemOpen(true);
					if (ImGui::TreeNode("entity_color", "Base Color"))
					{
						//ImGui::ColorPicker4("color_pick", objects[i]->EditColor().data());
						ImGui::ColorEdit4("Entity Color", objects[i]->EditColor().data());
						ImGui::TreePop();
					}

					if (ImGui::TreeNode("entity_delete", "Delete Entity"))
					{
						if (ImGui::Button("Delete"))
						{
							deleting = objects[i];
						}
						ImGui::TreePop();
					}

					ImGui::TreePop();
				}
				ImGui::PopID();
			}
			ImGui::TreePop();
		}

		ImGui::End();

		if (deleting != nullptr) HandleDelete(deleting);
	}

	void GuiManager::DrawToolbox()
	{
		ImGui::Begin("Toolbox", &ShowToolbox);

		if (PrimitiveNames.empty())
		{
			PrimitiveNames = App->GetPrimitiveGenerator().GetPrimitiveNames();
		}

		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNode("Primitive Type"))
		{
			int ctr = 0;
			for (const std::string& prim : PrimitiveNames)
			{
				ImGui::RadioButton(prim.c_str(), &PrimitiveIndex, ++ctr);
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("new_entity_props","Properties"))
		{
			if (ImGui::TreeNode("new_entity_pos", "Position: ")) { ImGui::TreePop(); }
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100.0f);
			if (ImGui::InputFloat("x", &DummyEntity->GetTransform().GetPosition().X(), 0.01f, 0.1f))
			{
				DummyEntity->UpdateTransform();
			}
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100.0f);
			if (ImGui::InputFloat("y", &DummyEntity->GetTransform().GetPosition().Y(), 0.01f, 0.1f))
			{
				DummyEntity->UpdateTransform();
			}
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100.0f);
			if (ImGui::InputFloat("z", &DummyEntity->GetTransform().GetPosition().Z(), 0.01f, 0.1f))
			{
				DummyEntity->UpdateTransform();
			}

			if (ImGui::TreeNode("new_entity_rot", "Rotation: ")) { ImGui::TreePop(); }
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100.0f);
			if (ImGui::SliderAngle("X", &DummyEntity->GetTransform().GetRotation().X()))
			{
				DummyEntity->GetTransform().UpdateTransform();
			}

			ImGui::SameLine();
			ImGui::SetNextItemWidth(100.0f);
			if (ImGui::SliderAngle("Y", &DummyEntity->GetTransform().GetRotation().Y()))
			{
				DummyEntity->GetTransform().UpdateTransform();
			}

			ImGui::SameLine();
			ImGui::SetNextItemWidth(100.0f);
			if (ImGui::SliderAngle("Z", &DummyEntity->GetTransform().GetRotation().Z()))
			{
				DummyEntity->GetTransform().UpdateTransform();
			}

			if (ImGui::TreeNode("new_entity_scale", "")) { ImGui::TreePop(); }
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100.0f);
			if (ImGui::DragFloat("Scale", &DummyEntity->GetTransform().GetScale(), 0.005f, 0.1f, 10.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp))
			{
				DummyEntity->GetTransform().UpdateTransform();
			}

			ImGui::SetNextItemOpen(true);
			if (ImGui::TreeNode("new_entity_color", "Base Color"))
			{
				//ImGui::ColorPicker4("color_pick", DummyEntity->EditColor().data());
				ImGui::ColorEdit4("Entity Color", DummyEntity->EditColor().data());
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		if(ImGui::Button("Create"))
		{
			HandleCreate();
		}
		
		ImGui::End();
	}

	void GuiManager::HandleCreate()
	{
		std::string key = PrimitiveNames[PrimitiveIndex-1];

		Material* mat = App->GetAppResources().GetMaterial("Wireframe");

		Vector4f dummy_pos = DummyEntity->GetPosition();

		auto builder = App->GetPrimitiveGenerator().RequestFactory(key)->Build();
		builder.Position(Vector3f(dummy_pos.X(), dummy_pos.Y(), dummy_pos.Z()))
			.Rotate(DummyEntity->GetRotation())
			.Scale(DummyEntity->GetScale())
			.WithColor(DummyEntity->GetColor())
			.WithMaterial(*mat)
			.OnScene(App->GetSceneManager().GetScene())
			.Finish();
	}

	void GuiManager::HandleDelete(SceneEntity* entity)
	{
		App->GetSceneManager().GetScene()->RemoveObject(entity);
	}

	void GuiManager::DrawMinimap()
	{
		ImGui::SetWindowPos("Minimap", ImVec2(0.8f, 0.2f), ImGuiCond_Once);
		ImGui::SetWindowSize("Minimap", ImVec2(100.f, 100.f), ImGuiCond_Once);
		ImGui::Begin("Minimap", &ShowMinimap);

		ImGui::End();
	}
}