/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Entity.cpp
* @brief 
* @date 1/2/2011
*/

#pragma once

#include <vector>
#include "Component.h"

namespace Etoile
{
	class RenderObject;
	class Entity
	{
	public:
		Entity(void);
		~Entity(void);
		const std::string getName(){return m_name;}
		void setName(const std::string& name){ m_name = name;}
		void setComponent(ComponentType type, Component*);
		Component* getComponent(ComponentType type){return m_components[type];}
		RenderObject* getRenderObject();
	private:
		std::vector<Component*> m_components;
		int m_id;
		std::string m_name;
	};
}
