#include <vector>
#include "EditorApplication.hpp"
#include "AppResources.hpp"
#include "../globj/Vertex.hpp"
#include "../scene/Transform.hpp"
#include "../scene/Mesh.hpp"

#include "../primitives/TriangleFactory.hpp"
#include "../primitives/DiamondFactory.hpp"
#include "../primitives/TetrahedronFactory.hpp"
#include "../primitives/CubeFactory.hpp"
#include "../primitives/RectangleFactory.hpp"
#include "../primitives/SphereFactory.hpp"
#include "PrimitiveGenerator.hpp"

#include <iostream>
#include <imgui_impl_sdl.h>

namespace av
{
	//Mesh* triangle_mesh = nullptr;
	Material* material = nullptr;

	SceneEntity* entity = nullptr;

	EditorApplication::EditorApplication(int argc, char* argv[])
		: Application(argc, argv), gui(this), scene(this)
	{
		primitives = new PrimitiveGenerator();
		test_director = new TestSceneDirector(*primitives);
		RegisterPrimitives();
	}

	void EditorApplication::RecomputeAspectRatio()
	{
		aspect_ratio = WinWidth / (float)WinHeight;
	}

	void EditorApplication::SetupTestScene()
	{
		test_director->BuildTestScene(*scene.GetScene(), *material);
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

		RecomputeAspectRatio();

		rotation_uniform = glGetUniformLocation(program->GetHandle(), "rm");
		aspect_uniform = glGetUniformLocation(program->GetHandle(), "aspect_ratio");

		rotation_matrix = GenerateRotationMatrix4f(0.0f, 0.0f, 0.0f);
		program->Bind();

		/*material = new Material();
		material->SetProgram(program);*/
		material = resources.GetMaterial("Wireframe");
		glUniform1fv(aspect_uniform, 1, &aspect_ratio);
		glUniformMatrix4fv(rotation_uniform, 1, false, rotation_matrix.data);

		SetupTestScene();
		ResetCameraPosition();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void EditorApplication::RegisterPrimitives()
	{
		primitives->RegisterFactory(new TriangleFactory(*primitives->GetVertexArray()));
		primitives->RegisterFactory(new DiamondFactory(*primitives->GetVertexArray()));
		primitives->RegisterFactory(new TetrahedronFactory(*primitives->GetVertexArray()));
		primitives->RegisterFactory(new CubeFactory(*primitives->GetVertexArray()));
		primitives->RegisterFactory(new RectangleFactory(*primitives->GetVertexArray()));
		primitives->RegisterFactory(new SphereFactory(*primitives->GetVertexArray()));
	}

	EditorApplication::~EditorApplication()
	{
		delete primitives;
		delete test_director;
	}

	void EditorApplication::HandleEvents()
	{
		while (SDL_PollEvent(&evt))
		{
			ImGui_ImplSDL2_ProcessEvent(&evt);

			switch (evt.type)
			{
			case SDL_WINDOWEVENT:
				switch (evt.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					ResizeWindow();
					break;
				}
				break;
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

	void EditorApplication::ResizeWindow()
	{
		SDL_GetWindowSize(window, (int*)&WinWidth, (int*)&WinHeight);
		glViewport(0, 0, WinWidth, WinHeight);
		RecomputeAspectRatio();
		scene.GetScene()->GetPerspective().SetAspectRatio(aspect_ratio);
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
		if (keys[SDL_SCANCODE_E]) { world->GetCamera().CamChangeDistance(-0.1f); }
		if (keys[SDL_SCANCODE_Q]) { world->GetCamera().CamChangeDistance(0.1f); }
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