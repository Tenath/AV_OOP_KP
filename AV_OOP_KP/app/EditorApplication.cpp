#include <vector>
#include "EditorApplication.hpp"
#include "AppResources.hpp"
#include "../globj/Vertex.hpp"
#include "../scene/Transform.hpp"
#include "../scene/Mesh.hpp"

#include "../primitives/TriangleBase.hpp"
#include "../primitives/DiamondBase.hpp"

#include <iostream>
#include <imgui_impl_sdl.h>

namespace av
{
	Mesh* BuildCubeMesh()
	{
		std::vector<Vertex> vertices =
		{
			Vertex(Vector3f(-1.0f, -1.0f, -1.0f), Vector4f(1.0f,1.0f,1.0f, 1.0f)),
			Vertex(Vector3f(-1.0f, -1.0f, 1.0f), Vector4f(1.0f,1.0f,1.0f, 1.0f)),
			Vertex(Vector3f(-1.0f, 1.0f, -1.0f), Vector4f(1.0f,1.0f,1.0f, 1.0f)),
			Vertex(Vector3f(-1.0f, 1.0f, 1.0f), Vector4f(1.0f,1.0f,1.0f, 1.0f)),
			Vertex(Vector3f(1.0f, -1.0f, -1.0f), Vector4f(1.0f,1.0f,1.0f, 1.0f)),
			Vertex(Vector3f(1.0f, -1.0f, 1.0f), Vector4f(1.0f,1.0f,1.0f, 1.0f)),
			Vertex(Vector3f(1.0f, 1.0f, -1.0f), Vector4f(1.0f,1.0f,1.0f, 1.0f)),
			Vertex(Vector3f(1.0f, 1.0f, 1.0f), Vector4f(1.0f,1.0f,1.0f, 1.0f)),
		};

		return nullptr;
	}

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

		rotation_uniform = glGetUniformLocation(program->GetHandle(), "rm");
		aspect_uniform = glGetUniformLocation(program->GetHandle(), "aspect_ratio");

		rotation_matrix = GenerateRotationMatrix4f(0.0f, 0.0f, 0.0f);
		program->Bind();

		material = new Material();
		material->SetProgram(program);
		glUniform1fv(aspect_uniform, 1, &aspect_ratio);
		glUniformMatrix4fv(rotation_uniform, 1, false, rotation_matrix.data);
		
		entity = primitive_builders["Diamond"]->Build()
			.Position(Vector3f(0.3f, 0.5f, 0.2f))
			.Rotate(Vector3f(0.2f, 0.0f, 0.0f))
			.WithColor(Vector4f(0.5f, 0.5f, 0.0f, 1.0f))
			.WithMaterial(*material)
			.Finish();

		program->Unbind();

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f);

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
		vertex_array->Bind();
		program->Bind();
		glUniformMatrix4fv(rotation_uniform, 1, false, rotation_matrix.data);
		
		entity->Draw();
		//vertex_array->Draw(0);

		vertex_array->Unbind();
		program->Unbind();

		gui.DrawGui();
	}

	void EditorApplication::HandleKeyboard()
	{
		if (SDL_GetKeyboardState(0)[SDL_SCANCODE_W]) { rotation.X() += 0.02f; }
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
		}
		if (SDL_GetKeyboardState(0)[SDL_SCANCODE_RETURN])
		{
			Matrix4f rot;
			glGetnUniformfv(program->GetHandle(), program->GetUniform("rm").Index, 16, rot.data);
			int n = 1;
		}
	}
	void EditorApplication::HandleMouse()
	{

	}
}