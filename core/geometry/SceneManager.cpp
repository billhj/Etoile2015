#include "SceneManager.h"

namespace Etoile
{
	SceneManager::SceneManager(void) : ResourceManager<Scene>()
	{
	}


	SceneManager::~SceneManager(void)
	{
	}

	void SceneManager::addScene(Scene* scene)
	{
		add(scene->getName(), scene);
	}

	void SceneManager::setCurrrentScene(Scene* current)
	{
		addScene(current);
		p_current = current;
	}

	Scene* SceneManager::getCurrentScene()
	{
		return p_current;
	}

}