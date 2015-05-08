#include "Scene.h"
#include "SceneManager.h"

namespace Etoile
{
	Scene::Scene(const std::string& name): m_name(name)
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

	void Scene::setName(const std::string& name)
	{
		SceneManager::getInstance()->renameElement(m_name, name);
		m_name = name;
	}
}