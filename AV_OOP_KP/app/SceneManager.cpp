#include "SceneManager.hpp"
#include "../scene/Scene.hpp"

namespace av
{
	void SceneManager::New() 
	{
		Discard();
		current_scene = new Scene();
	}

	void SceneManager::Discard()
	{
		delete current_scene;
	}
}
