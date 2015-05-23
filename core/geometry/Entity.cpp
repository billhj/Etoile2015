#include "Entity.h"
#include "ObjectRenderer.h"
#include "ModelTransform.h"

namespace Etoile
{
	static unsigned int ENTITY_ID = 0;
	Entity::Entity(const std::string& name) : m_name(name), m_visible(true)
	{
		m_id = ENTITY_ID++;
		m_components.resize(MAX_NB_COMPONENTS);
		setComponent(ComponentType::TRANSFORM_COMPONENT, new ModelTransform());
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

	void Entity::setVisible(bool visible)
	{
		m_visible = visible;
	}
}