/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ModelTransform.cpp
* @brief 
* @date 1/2/2011
*/

#include "ModelTransform.h"

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{
	/*ModelTransform::ModelTransform(): p_ref(NULL), Component(TRANSFORM_COMPONENT)
	{
		setScale(Vec3f(1,1,1));
		setTranslation(Vec3f());
		setRotation(Quaternionf());
		setTransform();
	}*/

	ModelTransform::ModelTransform(const ModelTransform& obj): Component(TRANSFORM_COMPONENT)
	{
		(*this) = obj;
		updateAll();
	}

	ModelTransform::ModelTransform(const Vec3f& translate, const Quaternionf& rotation, const Vec3f& scale, ModelTransform* ref):  p_ref(ref), Component(TRANSFORM_COMPONENT)
	{
	
		setScale(scale);
		setTranslation(translate);
		setRotation(rotation);
		updateAll();
	}

	void ModelTransform::set(const Vec3f& translate, const Quaternionf& rotation, const Vec3f& scale, ModelTransform* ref)
	{
		p_ref = ref;
		if(p_ref == NULL)
		{
			setScale(scale);
			setTranslation(translate);
			setRotation(rotation);
		}
		else
		{
			setScale(scale);
			setTranslation(translate);
			setRotation(rotation);
		}
		updateAll();
	}

	ModelTransform& ModelTransform::operator=(const ModelTransform& obj)
	{
	  // setTranslationAndRotation.
		setScale(obj.getScale());
		setTranslation(obj.getTranslation());
		setRotation(obj.getRotation());
		setReferenceObject(obj.getReferenceObject());
		updateAll();
		return *this;
	}

	void ModelTransform::setReferenceObject(ModelTransform* obj)
	{
		p_ref = obj;
	}

	void ModelTransform::setScale(const Vec3f& scale)
	{ 
		m_scale = scale;
	}

	void ModelTransform::setTranslation(const Vec3f& translation)
	{
		m_translation = translation;
	}

	void ModelTransform::translate(const Vec3f& translation)
	{
		m_translation += translation;
	}

	void ModelTransform::setRotation(const Quaternionf& rotation)
	{
		m_rotation = rotation;
		//m_rotation.normalize();
	}

	void ModelTransform::rotate(const Quaternionf& rotation)
	{
		m_rotation *= rotation;
		m_rotation.normalize();
	}

	Matrix4f ModelTransform::getModelMatrix(){return m_modelTransform;}
	Matrix4f ModelTransform::getGLModelMatrix()
	{
		Matrix4f model;
		m_modelTransform.transpose(model);
		return model;
	}

	void ModelTransform::setOrientation(const Quaternionf& orientation)
	{
		if(p_ref != NULL)
		{
			setRotation(p_ref->getOrientation().inverse() * orientation);
		}
		else
		{
			setRotation(orientation);
		}
	}

	void ModelTransform::setPosition(const Vec3f& pos)
	{
		if(p_ref != NULL)
		{
			Vec3f a = pos - p_ref->getPosition();
			Vec3f b = p_ref->getOrientation().inverse() * a;
			Vec3f c = b.scale(1.0 / p_ref->getScale().x(), 1.0 / p_ref->getScale().y(), 1.0 / p_ref->getScale().z());
			setTranslation(c);
		}
		else
		{
			setTranslation(pos);
		}
	}

	Quaternionf ModelTransform::getOrientation()
	{
		return m_orientation;
	}

	Vec3f ModelTransform::getPosition()
	{
		return m_position;
	}

	void ModelTransform::updateOrientation()
	{
		if(p_ref != NULL)
		{
			m_orientation = p_ref->getOrientation() * m_rotation;
		}
		else
		{
			m_orientation = m_rotation;
		}
	}

	void ModelTransform::updatePosition()
	{
		if(p_ref != NULL)
		{
			m_position = p_ref->getPosition() + p_ref->getOrientation() * (m_translation.scale(p_ref->getScale()));
		}
		else
		{
			m_position = m_translation;
		}
	}

	void ModelTransform::updateAll()
	{
		updateOrientation();
		updatePosition();
		updateTransform();
	}
}