#include <iostream>
#include "AppResources.hpp"

namespace av
{
	std::string AppResourceConfig::GetManifestPath(ResourceType type)
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

	std::string AppResourceConfig::AppendBaseDir(std::string path)
	{
		return BaseDirectory + "/" + path;
	}

	AppResources::~AppResources()
	{
		for (auto program : programs) delete program.second;
		for (auto shader : shaders) delete shader.second;
		for (auto model : models) delete model.second;
		for (auto scene : scenes) delete scene.second;
	}

	/*Model* AppResources::LoadModel(const std::string& filename)
	{
		Model* result = GetModel(filename);
		return (result != nullptr) ? result : _LoadModel(filename);
	}*/

	Model* AppResources::GetModel(const std::string& filename)
	{
		auto search = models.find(filename);
		//return (search != models.end()) ? search->second : _LoadModel(filename);
		return (search != models.end()) ? search->second : nullptr;
	}

	// Внутренняя процедура
	/*Model* AppResources::_LoadModel(const std::string& filename)
	{
		Model* result = nullptr;

		result = new Model(filename);
		models[filename] = result;

		return result;
	}*/

	Shader* AppResources::GetShader(const std::string& filename)
	{
		auto search = shaders.find(filename);
		return (search != shaders.end()) ? search->second : nullptr;
	}

	Program* AppResources::GetProgram(const std::string& name)
	{
		auto search = programs.find(name);
		return (search != programs.end()) ? search->second : nullptr;
	}

	Scene* AppResources::GetScene(const std::string& name)
	{
		auto search = scenes.find(name);
		return (search != scenes.end()) ? search->second : nullptr;
	}

	Material* AppResources::GetMaterial(const std::string& name)
	{
		auto search = materials.find(name);
		return (search != materials.end()) ? search->second : nullptr;
	}

	void AppResources::LoadResources()
	{
		LoadShaders();
		LoadPrograms();
		//LoadModels();
		LoadMaterials();
		LoadScenes();
	}

	void AppResources::LoadShaders()
	{
		std::vector<std::string> sh_lines = ReadTextFileLines(conf.GetManifestPath(ResourceType::Shader));

		std::string current_dir=".";

		size_t ctr = 0;
		for (const std::string& line : sh_lines)
		{
			++ctr;
			if (line.length() == 0) continue;
			try
			{
				if (line[0] == 'd')
				{
					std::vector<std::string> dirline = StringSplit(line, ',');
					if (dirline.size() < 2)
					{
						throw std::exception("Malformed directory line in shader manifest");
					}
					current_dir = dirline[1];
				}
				else if (line[0] == 's')
				{
					std::vector<std::string> shline = StringSplit(line, ',');
					if (shline.size() < 3)
					{
						throw std::exception("Malformed shader line in shader manifest");
					}
					std::string path = current_dir + "/" + shline[1];
					std::string fullpath = conf.AppendBaseDir(path);
					ShaderType type = Shader::ParseType(shline[2]);

					if (type == ShaderType::Undefined)
					{
						throw std::exception("Malformed shader type in manifest line");
					}

					if (MapContainsKey(shaders, path))
					{
						throw std::exception("Duplicate shader entry");
					}

					std::string src = ReadTextFile(fullpath);
					Shader* sh = new Shader(src, type);
					shaders[path] = sh;
				}
			}
			catch (const std::exception& e)
			{
				std::cout << conf.ShaderManifest << "#" + std::to_string(ctr) + ":" << e.what() << std::endl;
			}
		}
	}

	void AppResources::LoadPrograms()
	{
		std::vector<std::string> pr_lines = ReadTextFileLines(conf.GetManifestPath(ResourceType::Program));
		
		std::map<std::string, std::vector<std::string>> defined_programs;

		std::string current_prog = "";
		size_t ctr = 0;
		// Формируем список программ и входящих в них шейдеров из манифеста
		for (const std::string& line : pr_lines)
		{
			++ctr;
			if (line.length() == 0) continue;
			try
			{
				if (line[0] == 'p')
				{
					std::vector<std::string> progline = StringSplit(line, ',');
					if (progline.size() < 2) throw std::exception("Malformed program line in program manifest");
					current_prog = progline[1];
					defined_programs[current_prog] = std::vector<std::string>();
				}
				else if (line[0] == 's')
				{
					if (current_prog.length() == 0) throw std::exception("Shader line not associated with a program");
					std::vector<std::string> shline = StringSplit(line, ',');
					if (shline.size() < 2) throw std::exception("Malformed shader line in program manifest");
					defined_programs[current_prog].push_back(shline[1]);
				}
			}
			catch (const std::exception& e)
			{
				std::cout << conf.ProgramManifest << "#" + std::to_string(ctr) + ":" << e.what() << std::endl;
			}
		}

		// Собираем программы
		for (auto progentry : defined_programs)
		{
			Program* p = nullptr;
			try
			{
				if (GetProgram(progentry.first) != nullptr) 
					throw std::exception("Duplicate program name");
				p = new Program();
				for (const std::string& shfile : progentry.second)
				{
					Shader* shader = GetShader(shfile);
					if (shader == nullptr) throw std::exception("Shader not found");
					p->AttachShader(shader);
				}
				p->Build();

			}
			catch (const std::exception e)
			{
				std::cout << "Exception during "+progentry.first+" program assembly: " << e.what() << std::endl;
				if (p != nullptr) { delete p; p = nullptr; }
			}
			// Если всё получилось - добавляем запись в map
			if(p!=nullptr) programs[progentry.first] = p;
		}
	}

	/*void AppResources::LoadModels()
	{
		std::vector<std::string> m_lines = ReadTextFileLines(conf.ModelManifest);

		std::string current_dir = ".";

		size_t ctr = 0;
		for (const std::string& line : m_lines)
		{
			++ctr;
			if (line.length() == 0) continue;
			try
			{
				if (line[0] == 'd')
				{
					std::vector<std::string> dirline = StringSplit(line, ',');
					if (dirline.size() < 2)
						throw std::exception("Malformed directory line in model manifest");
					current_dir = dirline[1];
				}
				else if (line[0] == 'm')
				{
					std::vector<std::string> shline = StringSplit(line, ',');
					if (shline.size() < 2)
						throw std::exception("Malformed model line in model manifest");
					std::string path = current_dir + "\\" + shline[1];

					if (MapContainsKey(models, path))
						throw std::exception("Duplicate model entry");

					Model* m = LoadModel(path);
					models[path] = m;
				}
			}
			catch (const std::exception& e)
			{
				std::cout << conf.ModelManifest << "#" + std::to_string(ctr) + ":" << e.what() << std::endl; 
			}
		}
	}*/

	void AppResources::LoadScenes()
	{
		std::vector<std::string> s_lines = ReadTextFileLines(conf.SceneManifest);

		for (const std::string& line : s_lines)
		{
			LoadScene(line);
		}
	}

	Scene* AppResources::LoadScene(const std::string& filename)
	{
		Scene* existing_scene = GetScene(filename);
		if (existing_scene != nullptr) return existing_scene;

		std::vector<std::string> s_lines = ReadTextFileLines(filename);

		Scene* world = new Scene;
		scenes[filename] = world;

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

					Model* mod = GetModel(objline[1]);
					if (mod == nullptr)
						throw std::exception(("Model \"" + objline[1] + "\" not found!").c_str());

					Material* mat = GetMaterial(objline[2]);

					Vector3f pos = UnpackVectorString<3>(objline[3]);
					Vector3f rot = UnpackVectorString<3>(objline[4]);
					float scale = std::stof(objline[5]);
					Vector4f color = UnpackVectorString<4>(objline[6]);
					
					SceneEntity* obj = new SceneEntity(mod,pos);
					obj->SetRotation(rot);
					obj->SetScale(scale);
					obj->SetColor(color);
					obj->SetMaterial(mat);
					world->AddObject(obj);
				}
				// Case: Light Source
				if (line[0] == 'l')
				{
					auto lsline = StringSplit(line, ';');
					if (lsline.size() != 8)
						throw std::exception("Malformed light source line in scene file");

					Vector3f pos = UnpackVectorString<3>(lsline[1]);
					float constant = std::stof(lsline[2]);
					float linear = std::stof(lsline[3]);
					float quadratic = std::stof(lsline[4]);
					Vector3f ambient = UnpackVectorString<3>(lsline[5]);
					Vector3f diffuse = UnpackVectorString<3>(lsline[6]);
					Vector3f specular = UnpackVectorString<3>(lsline[7]);
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

	void AppResources::LoadMaterials()
	{
		std::vector<std::string> m_lines = ReadTextFileLines(conf.GetManifestPath(ResourceType::Material));
		size_t ctr=1;
		for (const std::string& line : m_lines)
		{
			try
			{
				if (line.size() == 0) continue;
				// Case: Object
				if (line[0] == 'm')
				{
					auto matline = StringSplit(line, ';');
					if (matline.size() != 7)
						throw std::exception("Malformed line in materials file");

					std::string name = matline[1];
					std::string progname = matline[2];
					Vector3f ambient = UnpackVectorString<3>(matline[3]);
					Vector3f diffuse = UnpackVectorString<3>(matline[4]);
					Vector3f specular = UnpackVectorString<3>(matline[5]);
					float shininess = std::stof(matline[6]);
					
					Material* mat = new Material;
					mat->SetProgram(GetProgram(progname));
					mat->Name = name;
					mat->Ambient = ambient;
					mat->Diffuse = diffuse;
					mat->Specular = specular;
					mat->Shininess = shininess;
					
					materials[name] = mat;
				}
			}
			catch (const std::exception& e)
			{
				std::cout << "(Material)" << conf.MaterialManifest << "#" + std::to_string(ctr) + ":" << e.what() << std::endl;
			}
			++ctr;
		}
	}
}