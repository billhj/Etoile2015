#include "Scene.h"
#include "SceneManager.h"

namespace Etoile
{
	Scene::Scene(void)
	{
		SceneManager::getInstance()->addScene(this);
	}


	Scene::~Scene(void)
	{
	}

	void Scene::addEntity(Entity* entity)
	{
		m_entities.push_back(entity);
	}
}