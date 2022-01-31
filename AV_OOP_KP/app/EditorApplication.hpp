#pragma once
#include <map>
#include "../math/av_vector_t.hpp"
#include "../math/av_matrix_t.hpp"
#include "../globals.hpp"
#include "../globj/Program.hpp"
#include "../globj/VertexArray.hpp"
#include "../globj/Vertex.hpp"
#include "Application.hpp"
#include "GuiManager.hpp"
#include "SceneManager.hpp"
#include "AppResources.hpp"

namespace av
{
	class PrimitiveBase;
	class AppResources;
	class PrimitiveGenerator;

	class EditorApplication : public Application 
	{
	private:
		PrimitiveGenerator* primitives = nullptr;
		Program* program = nullptr;

		Vector3f rotation;

		Matrix4f rotation_matrix;

		GLuint rotation_uniform = 0;
		GLuint aspect_uniform = 0;

		float aspect_ratio = 1.0f;

		GuiManager gui;
		SceneManager scene;
		AppResources resources;

		void RecomputeAspectRatio();
		void ResizeWindow();
		void RegisterPrimitives();
	public:
		void AppInit() override;

		void HandleEvents() override;
		void HandleKeyboard();
		void HandleMouse();

		void Process() override;

		void Draw() override;

		EditorApplication(int argc, char* argv[]);

		~EditorApplication();

		GuiManager& GetGuiManager() { return gui; }
		SceneManager& GetSceneManager() { return scene; }
		PrimitiveGenerator& GetPrimitiveGenerator() { return *primitives; }
		AppResources& GetAppResources() { return resources; }

		void ResetCameraPosition();
	};
}