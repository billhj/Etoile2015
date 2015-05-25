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

	int Scene::addEntity(Entity* entity)
	{
		for(unsigned int i = 0; i < m_entities.size(); ++i)
		{
			if(m_entities[i] == entity)
			{
				m_entities.erase(m_entities.begin() + i);
				return i;
			}
		}
		int size = m_entities.size();
		m_entities.push_back(entity);
		return size;
	}

	void Scene::removeEntity(Entity* entity)
	{
		for(unsigned int i = 0; i < m_entities.size(); ++i)
		{
			if(m_entities[i] == entity)
			{
				m_entities.erase(m_entities.begin() + i);
				return;
			}
		}
	}

	void Scene::setName(const std::string& name)
	{
		SceneManager::getInstance()->renameElement(m_name, name);
		m_name = name;
	}

	Entity* Scene::getEntity(int idx)
	{
		return m_entities[idx];
	}
}