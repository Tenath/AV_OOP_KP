#include <vector>
#include "EditorApplication.hpp"
#include "../globj/Vertex.hpp"
#include "../scene/Transform.hpp"
#include "../scene/Mesh.hpp"

#include <iostream>

namespace av
{
	Mesh* BuildTriangleMesh()
	{
		std::vector<Vertex> vertices =
		{
			Vertex(Vector3f(-0.5f, -0.3f, 0.0f), Vector4f(1.0f,0.0f,0.0f,1.0f)),
			Vertex(Vector3f(0.5f, -0.3f, 0.0f), Vector4f(0.0f,1.0f,0.0f,1.0f)),
			Vertex(Vector3f(0.0f, 0.6f, 0.0f), Vector4f(0.0f,0.0f,1.0f,1.0f)),
		};

		std::vector<ushort> indices = { 0,1,2 };

		PrimitiveType type = PrimitiveType::Triangles;

		Mesh* tri_mesh = new Mesh(
			"triangle",
			vertices,
			indices,
			PrimitiveType::Triangles,
			nullptr,
			nullptr
		);

		return tri_mesh;
	}

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

	Mesh* triangle_mesh = nullptr;
	Material* material = nullptr;

	void EditorApplication::RecomputeAspectRatio()
	{
		aspect_ratio = WinWidth / (float)WinHeight;
	}

	void EditorApplication::AppInit()
	{
		Application::AppInit();

		SetTargetFPS(144);
		program = new Program("data/shaderlist.txt");

		if (!program->IsBuilt())
		{
			throw std::logic_error("Error in shader program: " + program->GetError());
		}

		//DrawSequence<Vertex, ushort> triangle(PrimitiveType::Triangles, { 0,1,2 });
		//std::vector<DrawSequence<unsigned>> sequences = { triangle };
		triangle_mesh = BuildTriangleMesh();

		RecomputeAspectRatio();
		vertex_array = new VertexArray<Vertex, ushort>();
		triangle_mesh->SetupVertexArray(*vertex_array);

		rotation_uniform = glGetUniformLocation(program->GetHandle(), "rm");
		aspect_uniform = glGetUniformLocation(program->GetHandle(), "aspect_ratio");

		rotation_matrix = GenerateRotationMatrix4f(0.0f, 0.0f, 0.0f);
		program->Bind();

		material = new Material();
		material->SetProgram(program);
		glUniform1fv(aspect_uniform, 1, &aspect_ratio);
		glUniformMatrix4fv(rotation_uniform, 1, false, rotation_matrix.data);
		
		program->Unbind();
	}

	EditorApplication::~EditorApplication()
	{
		delete triangle_mesh;
		delete vertex_array;
		delete material;
		delete program;
	}

	void EditorApplication::HandleEvents()
	{
		while (SDL_PollEvent(&evt))
		{
			switch (evt.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			default:
				break;
			}
		}

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

	void EditorApplication::Process()
	{
		rotation_matrix = GenerateRotationMatrix4f(rotation.X(), rotation.Y(), rotation.Z());
	}

	void EditorApplication::Draw()
	{
		Application::Draw();
		vertex_array->Bind();
		program->Bind();
		glUniformMatrix4fv(rotation_uniform, 1, false, rotation_matrix.data);

		vertex_array->Draw(0);

		vertex_array->Unbind();
		program->Unbind();
	}
}