#include <iostream>
#include <sstream>
#include "SceneManager.hpp"
#include "../scene/Scene.hpp"
#include "EditorApplication.hpp"
#include "PrimitiveGenerator.hpp"

namespace av
{
	void SceneManager::New() 
	{
		Discard();
		current_scene = new Scene();
		app->ResetCameraPosition();
	}

	void SceneManager::Open(const std::string& filename)
	{
		Discard();
		current_scene = LoadScene(filename);
		this->filename = filename;
		app->ResetCameraPosition();
	}

	void SceneManager::Save()
	{
		SaveScene(filename, current_scene);
	}

	void SceneManager::SaveAs(const std::string& filename)
	{
		SaveScene(filename, current_scene);
		this->filename = filename;
	}

	void SceneManager::Close() { New(); };

	SceneManager::SceneManager(EditorApplication* p_app)
	{
		app = p_app;
		New();
	}

	void SceneManager::Discard()
	{
		delete current_scene;
		current_scene = nullptr;
	}

	void SceneManager::SaveScene(const std::string& filename, Scene* scene)
	{
		std::vector<std::string> s_lines;

		std::ostringstream strstr;

		for (SceneEntity* obj : scene->GetObjects())
		{
			std::string pos = PackVectorString(obj->GetPosition());
			std::string rot = PackVectorString(obj->GetRotation());
			std::string scale = std::to_string(obj->GetScale());
			std::string color = PackVectorString(obj->GetColor());

			strstr << "o;" << obj->GetModel()->GetName() << ";" << 
				obj->GetMaterial()->Name << ";"
				<< pos << ";" << rot << ";" << scale 
				<< ";" << color << std::endl;
			
			s_lines.push_back(strstr.str());

			strstr.str("");
		}

		WriteTextFileLines(filename, s_lines);
	}

	Scene* SceneManager::LoadScene(const std::string& filename) 
	{
		std::vector<std::string> s_lines = ReadTextFileLines(filename);

		Scene* world = new Scene;

		size_t ctr = 1;
		for (const std::string& line : s_lines)
		{
			try
			{
				if (line.size() == 0) continue;
				// Case: Object
				if (line[0] == 'o')
				{
					auto objline = StringSplit(line, ';');
					if (objline.size() != 7)
						throw std::exception("Malformed object line in scene file");

					PrimitiveFactory* base = app->GetPrimitiveGenerator().RequestFactory(objline[1]);
					if (base == nullptr)
						throw std::exception(("Primitive \"" + objline[1] + "\" not found!").c_str());

					Material* mat = app->GetAppResources().GetMaterial(objline[2]);

					Vector3f pos = UnpackVectorString<3>(objline[3]);
					Vector3f rot = UnpackVectorString<3>(objline[4]);
					float scale = std::stof(objline[5]);
					Vector4f color = UnpackVectorString<4>(objline[6]);

					SceneEntity* obj = base->Build()
						.Position(pos)
						.Rotate(rot)
						.Scale(scale)
						.WithColor(color)
						.WithMaterial(*mat)
						.OnScene(world)
						.Finish();
				}
			}
			catch (const std::exception& e)
			{
				std::cout << "(Scene)" << filename << "#" + std::to_string(ctr) + ":" << e.what() << std::endl;
			}
			++ctr;
		}

		return world;
	}

	std::string SceneManager::GetActiveFile() { return filename; }
	Scene* SceneManager::GetScene() { return current_scene; }
	void SceneManager::Draw() { if (current_scene != nullptr) current_scene->Draw(); }
}
