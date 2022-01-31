#pragma once
#include <map>
#include <string>
#include "../globj/Program.hpp"
#include "../scene/Scene.hpp"

namespace av
{
	enum class ResourceType
	{
		Shader,
		Program,
		Model,
		Scene,
		Material
	};

	struct AppResourceConfig
	{
		std::string BaseDirectory = "data";
		std::string ShaderManifest = "shaderslist.cfg";
		std::string ProgramManifest = "programslist.cfg";
		std::string ModelManifest = "modelslist.cfg";
		std::string SceneManifest = "scenelist.cfg";
		std::string MaterialManifest = "materialslist.cfg";

		std::string GetManifestPath(ResourceType type)
		{
			std::string result = BaseDirectory + "/";

			switch (type)
			{
			case ResourceType::Shader: result += ShaderManifest; break;
			case ResourceType::Program: result += ProgramManifest; break;
			case ResourceType::Model: result += ModelManifest; break;
			case ResourceType::Scene: result += SceneManifest; break;
			case ResourceType::Material: result += MaterialManifest; break;
			}

			return result;
		}

		std::string AppendBaseDir(std::string path)
		{
			return BaseDirectory + "/" + path;
		}
	};

	class AppResources
	{
	private:
		AppResourceConfig conf;

		std::map<std::string, GLuint> uniforms;
		std::map<std::string, Program*> programs;
		std::map<std::string, Shader*> shaders;
		std::map<std::string, Model*> models;
		std::map<std::string, Scene*> scenes;
		std::map<std::string, Material*> materials;

		//Model* _LoadModel(const std::string& filename);
		Scene* LoadScene(const std::string& filename);

		void LoadShaders();
		void LoadPrograms();
		//void LoadModels();
		void LoadScenes();
		void LoadMaterials();
	public:
		void LoadResources();

		~AppResources()
		{
			for (auto program : programs) delete program.second;
			for (auto shader : shaders) delete shader.second;
			for (auto model : models) delete model.second;
			for (auto scene : scenes) delete scene.second;
		}

		Model* GetModel(const std::string& filename);
		//Model* LoadModel(const std::string& filename);

		Shader* GetShader(const std::string& filename);
		Program* GetProgram(const std::string& name);

		Scene* GetScene(const std::string& scene);
		Material* GetMaterial(const std::string& material);
	};
}