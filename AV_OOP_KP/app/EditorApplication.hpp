#pragma once
#include "Application.hpp"
#include "GuiManager.hpp"
#include "../math/av_vector_t.hpp"
#include "../math/av_matrix_t.hpp"
#include "../globals.hpp"
#include "../globj/Program.hpp"
#include "../globj/VertexArray.hpp"
#include "../globj/Vertex.hpp"

namespace av
{
	class EditorApplication : public Application 
	{
	private:
		VertexArray<Vertex, ushort>* vertex_array = nullptr;
		Program* program = nullptr;

		Vector3f rotation;

		Matrix4f rotation_matrix;

		GLuint rotation_uniform = 0;
		GLuint aspect_uniform = 0;

		float aspect_ratio = 1.0f;

		GuiManager gui;

		void RecomputeAspectRatio();
	public:
		void AppInit() override;

		void HandleEvents() override;

		void Process() override;

		void Draw() override;

		EditorApplication(int argc, char* argv[]) : Application(argc,argv)
		{

		}

		~EditorApplication();
	};
}