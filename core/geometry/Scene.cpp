#include "Scene.h"

namespace Etoile
{
	static unsigned int SCENE_ID = 0;
	Scene::Scene(void)
	{
		m_id = SCENE_ID++;
	}


	Scene::~Scene(void)
	{
	}

	void Scene::addEntity(Entity* entity)
	{
		m_entities.push_back(entity);
	}
}