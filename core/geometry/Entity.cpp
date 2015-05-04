#include "Entity.h"
#include "ObjectRenderer.h"
#include "ModelTransform.h"

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
		c->setEntity(this);
	}

	ObjectRenderer* Entity::getObjectRenderer()
	{
		return (ObjectRenderer*)(m_components[ComponentType::RENDER_COMPONENT]);
	}

	ModelTransform* Entity::getTransformation()
	{
		return (ModelTransform*)(m_components[ComponentType::TRANSFORM_COMPONENT]);
	}
}