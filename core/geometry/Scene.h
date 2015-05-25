/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Scene.cpp
* @brief 
* @date 1/2/2011
*/

#pragma once

#include "entity.h"

namespace Etoile
{
	class Scene
	{
	public:
		Scene(const std::string& name);
		~Scene(void);
		const std::string getName(){return m_name;}
		void setName(const std::string& name);
		int addEntity(Entity*);
		Entity* getEntity(int idx);
		void removeEntity(Entity*);
		std::vector<Entity*>& getEntities(){return m_entities;}
		const std::vector<Entity*>& getEntities() const {return m_entities;}
	private:
		std::vector<Entity*> m_entities;
		std::string m_name;
	};

}