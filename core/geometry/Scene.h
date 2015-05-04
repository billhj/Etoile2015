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
		Scene(void);
		~Scene(void);
		const std::string getName(){return m_name;}
		void setName(const std::string& name){ m_name = name;}
		void addEntity(Entity*);
		std::vector<Entity*>& getEntities(){return m_entities;}
		const std::vector<Entity*>& getEntities() const {return m_entities;}
	private:
		std::vector<Entity*> m_entities;
		int m_id;
		std::string m_name;
	};

}