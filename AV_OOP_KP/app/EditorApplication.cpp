#include <vector>
#include "EditorApplication.hpp"
#include "AppResources.hpp"
#include "../globj/Vertex.hpp"
#include "../scene/Transform.hpp"
#include "../scene/Mesh.hpp"

#include "../primitives/TriangleBase.hpp"
#include "../primitives/DiamondBase.hpp"
#include "../primitives/TetrahedronBase.hpp"
#include "../primitives/CubeBase.hpp"

#include <iostream>
#include <imgui_impl_sdl.h>

namespace av
{
	//Mesh* triangle_mesh = nullptr;
	Material* material = nullptr;

	SceneEntity* entity = nullptr;

	EditorApplication::EditorApplication(int argc, char* argv[])
		: Application(argc, argv), gui(this)
	{

	}

	void EditorApplication::RecomputeAspectRatio()
	{
		aspect_ratio = WinWidth / (float)WinHeight;
	}

	void EditorApplication::ResetCameraPosition()
	{
		Scene& s = *scene.GetScene();

		Camera& cam = s.GetCamera();
		cam.ShiftFocus({ 0.0f,0.0f,0.0f });
		cam.CamSetDistance(3.0f);
		cam.CamSetHorizontal(PI / 4);
		cam.CamSetVertical(PI / 4);
	}

	void EditorApplication::AppInit()
	{
		Application::AppInit();
		resources.LoadResources();

		SetTargetFPS(144);
		//program = new Program("data/shaderlist.txt");
		program = resources.GetProgram("Wireframe");

		if (!program->IsBuilt())
		{
			throw std::logic_error("Error in shader program: " + program->GetError());
		}

		//DrawSequence<Vertex, ushort> triangle(PrimitiveType::Triangles, { 0,1,2 });
		//std::vector<DrawSequence<unsigned>> sequences = { triangle };
		//triangle_mesh = BuildTriangleMesh();

		RecomputeAspectRatio();
		vertex_array = new VertexArray<Vertex, ushort>();
		
		primitive_builders["Triangle"] = new TriangleBase(*vertex_array);
		primitive_builders["Diamond"] = new DiamondBase(*vertex_array);
		primitive_builders["Tetrahedron"] = new TetrahedronBase(*vertex_array);
		primitive_builders["Cube"] = new CubeBase(*vertex_array);

		rotation_uniform = glGetUniformLocation(program->GetHandle(), "rm");
		aspect_uniform = glGetUniformLocation(program->GetHandle(), "aspect_ratio");

		rotation_matrix = GenerateRotationMatrix4f(0.0f, 0.0f, 0.0f);
		program->Bind();

		material = new Material();
		material->SetProgram(program);
		glUniform1fv(aspect_uniform, 1, &aspect_ratio);
		glUniformMatrix4fv(rotation_uniform, 1, false, rotation_matrix.data);
		
		entity = primitive_builders["Tetrahedron"]->Build()
			.Position(Vector3f(0.0f, 0.0f, 0.0f))
			.Rotate(Vector3f(0.0f, 0.0f, 0.0f))
			.Scale(0.4f)
			.WithColor(Vector4f(0.9f, 0.3f, 0.0f, 1.0f))
			.WithMaterial(*material)
			.OnScene(scene.GetScene())
			.Finish();

		entity = primitive_builders["Triangle"]->Build()
			.Position(Vector3f(0.5f, 0.0f, 0.0f))
			.Rotate(Vector3f(0.0f, 0.0f, 0.0f))
			.Scale(0.5f)
			.WithColor(Vector4f(0.7f, 0.8f, 0.4f, 1.0f))
			.WithMaterial(*material)
			.OnScene(scene.GetScene())
			.Finish();

		entity = primitive_builders["Diamond"]->Build()
			.Position(Vector3f(0.0f, 0.0f, 0.5f))
			.Rotate(Vector3f(0.0f, 0.0f, 0.0f))
			.Scale(0.5f)
			.WithColor(Vector4f(0.0f, 0.8f, 0.8f, 1.0f))
			.WithMaterial(*material)
			.OnScene(scene.GetScene())
			.Finish();

		entity = primitive_builders["Cube"]->Build()
			.Position(Vector3f(-0.5f, 0.0f, -0.5f))
			.Rotate(Vector3f(0.0f, 0.0f, 0.0f))
			.Scale(0.25f)
			.WithColor(Vector4f(0.2f, 0.8f, 0.4f, 1.0f))
			.WithMaterial(*material)
			.OnScene(scene.GetScene())
			.Finish();

		//program->Unbind();

		ResetCameraPosition();

		/*glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f);*/

		//glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	EditorApplication::~EditorApplication()
	{
		delete entity;

		for (auto entry : primitive_builders)
		{
			delete entry.second;
		}

		delete vertex_array;
		delete material;
		//delete program;

		primitive_builders.clear();
	}

	void EditorApplication::HandleEvents()
	{
		while (SDL_PollEvent(&evt))
		{
			ImGui_ImplSDL2_ProcessEvent(&evt);

			switch (evt.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			default:
				break;
			}
		}

		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureKeyboard) { HandleKeyboard(); }
		if (!io.WantCaptureMouse) { HandleMouse(); }
	}

	void EditorApplication::Process()
	{
		rotation_matrix = GenerateRotationMatrix4f(rotation.X(), rotation.Y(), rotation.Z());
	}

	void EditorApplication::Draw()
	{
		// Заливка задника цветом, "обнуление" (или, если хотите, "объединичивание") глубинного буффера
		glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Application::Draw();
		/*vertex_array->Bind();
		program->Bind();
		glUniformMatrix4fv(rotation_uniform, 1, false, rotation_matrix.data);
		
		entity->Draw();
		//vertex_array->Draw(0);

		vertex_array->Unbind();
		program->Unbind();*/

		scene.Draw();
		gui.DrawGui();
	}

	void EditorApplication::HandleKeyboard()
	{
		/*if (SDL_GetKeyboardState(0)[SDL_SCANCODE_W]) { rotation.X() += 0.02f; }
		if (SDL_GetKeyboardState(0)[SDL_SCANCODE_S]) { rotation.X() -= 0.02f; }
		if (SDL_GetKeyboardState(0)[SDL_SCANCODE_A]) { rotation.Y() += 0.02f; }
		if (SDL_GetKeyboardState(0)[SDL_SCANCODE_D]) { rotation.Y() -= 0.02f; }
		if (SDL_GetKeyboardState(0)[SDL_SCANCODE_Q]) { rotation.Z() += 0.02f; }
		if (SDL_GetKeyboardState(0)[SDL_SCANCODE_E]) { rotation.Z() -= 0.02f; }
		if (SDL_GetKeyboardState(0)[SDL_SCANCODE_BACKSPACE])
		{
			rotation.X() = 0.0f;
			rotation.Y() = 0.0f;
			rotation.Z() = 0.0f;
		}*/
		const Uint8* keys = SDL_GetKeyboardState(nullptr);

		Scene* world = scene.GetScene();

		if (keys[SDL_SCANCODE_A]) { world->GetCamera().SidestepFocus(-0.1f); }
		if (keys[SDL_SCANCODE_D]) { world->GetCamera().SidestepFocus(0.1f); }
		if (keys[SDL_SCANCODE_W]) { world->GetCamera().AdvanceFocus(-0.1f); }
		if (keys[SDL_SCANCODE_S]) { world->GetCamera().AdvanceFocus(0.1f); }
		if (keys[SDL_SCANCODE_KP_PLUS]) { world->GetCamera().CamChangeDistance(-0.1f); }
		if (keys[SDL_SCANCODE_KP_MINUS]) { world->GetCamera().CamChangeDistance(0.1f); }
		if (keys[SDL_SCANCODE_BACKSPACE]) { ResetCameraPosition(); }

		if (keys[SDL_SCANCODE_UP]) { world->GetCamera().CamRotateVertical(0.05f); }
		if (keys[SDL_SCANCODE_DOWN]) { world->GetCamera().CamRotateVertical(-0.05f); }
		if (keys[SDL_SCANCODE_LEFT]) { world->GetCamera().CamRotateHorizontal(0.05f); }
		if (keys[SDL_SCANCODE_RIGHT]) { world->GetCamera().CamRotateHorizontal(-0.05f); }

		if (keys[SDL_SCANCODE_RETURN])
		{
			Matrix4f rot;
			glGetnUniformfv(program->GetHandle(), program->GetUniform("model_matrix").Location, 16, rot.data);
			/*for (auto entry : program->GetUniforms())
			{
				switch (entry.second.Type)
				{

				}
				
			}*/
			
			int n = 1;
		}
	}
	void EditorApplication::HandleMouse()
	{

	}
}