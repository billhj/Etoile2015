#include "Entity.h"
#include "RenderObject.h"

namespace Etoile
{
	static unsigned int ENTITY_ID = 0;
	Entity::Entity(void)
	{
		m_id = ENTITY_ID++;
		m_components.resize(MAX_NB_COMPONENTS);
	}


	Entity::~Entity(void)
	{
	}


	void Entity::setComponent(ComponentType type, Component* c)
	{
		m_components.insert(m_components.begin() + type, c);
	}

	RenderObject* Entity::getRenderObject()
	{
		return (RenderObject*)(m_components[ComponentType::RENDER_OBJECT_COMPONENT]);
	}
}