#include "SceneManager.h"

namespace Etoile
{
	SceneManager::SceneManager(void)
	{
	}


	SceneManager::~SceneManager(void)
	{
	}

	void SceneManager::addScene(Scene* scene)
	{
		m_scenes.push_back(scene);
	}

	void SceneManager::setCurrrentScene(Scene* current)
	{
		p_current = current;
	}

	Scene* SceneManager::getCurrentScene()
	{
		return p_current;
	}

}