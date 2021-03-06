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
	class Scene;
	class ObjectRenderer;
	class ModelTransform;
	class Entity
	{
	public:
		Entity(const std::string& name ="", Scene* owner= NULL);
		~Entity(void);
		const std::string getName(){return m_name;}
		void setName(const std::string& name){ m_name = name;}
		void setComponent(ComponentType type, Component*);
		Component* getComponent(ComponentType type){return m_components[type];}
		ObjectRenderer* getObjectRenderer();
		ModelTransform* getTransformation();
		void setVisible(bool visible);
		bool isVisible(){return m_visible;}
		Scene* getScene(){return p_owner;}
		int getIndex(){return m_id;}
	private:
		std::vector<Component*> m_components;
		int m_id;
		std::string m_name;
		bool m_visible;
		Scene* p_owner;
	};
}
