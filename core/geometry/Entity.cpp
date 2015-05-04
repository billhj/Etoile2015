#include "Entity.h"
#include "ObjectRenderer.h"

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

	ObjectRenderer* Entity::getObjectRenderer()
	{
		return (ObjectRenderer*)(m_components[ComponentType::RENDER_OBJECT_COMPONENT]);
	}
}