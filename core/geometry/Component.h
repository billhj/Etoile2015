/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Component.cpp
* @brief 
* @date 1/2/2011
*/

#pragma once

namespace Etoile
{
	enum ComponentType {MESH_COMPONENT, RENDER_COMPONENT, TRANSFORM_COMPONENT, MANIPULATOR_COMPONENT};
#define MAX_NB_COMPONENTS 10

	class Entity;
	class Component
	{
	public:
		Component(ComponentType type){m_type = type;}
		~Component(void){}
		ComponentType getComponentType(){return m_type;} 
		void setEntity(Entity* entity)
		{
			p_entity = entity;		
		}
		Entity* getEntity()
		{
			return p_entity;
		}
	private:
		ComponentType m_type;
		Entity* p_entity;
	};
}
