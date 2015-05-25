#include "Entity.h"
#include "ObjectRenderer.h"
#include "ModelTransform.h"
#include "Scene.h"

namespace Etoile
{
	Entity::Entity(const std::string& name, Scene* owner) : m_name(name), m_visible(true), p_owner(owner)
	{
		if(NULL != owner)
		{
			m_id = owner->addEntity(this);
		}
		m_components.resize(MAX_NB_COMPONENTS);
		setComponent(ComponentType::TRANSFORM_COMPONENT, new ModelTransform());
	}


	Entity::~Entity(void)
	{
	}


	void Entity::setComponent(ComponentType type, Component* c)
	{
		m_components[type] = c;
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