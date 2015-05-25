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
		setTransform();
	}

	ModelTransform::ModelTransform(const Vec3f& translate, const Quaternionf& rotation, const Vec3f& scale, ModelTransform* ref):  p_ref(ref), Component(TRANSFORM_COMPONENT)
	{
	
		setScale(scale);
		setTranslation(translate);
		setRotation(rotation);
		setTransform();
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
	}

	ModelTransform& ModelTransform::operator=(const ModelTransform& obj)
	{
	  // setTranslationAndRotation.
		setScale(obj.getScale());
		setTranslation(obj.getTranslation());
		setRotation(obj.getRotation());
		setReferenceObject(obj.getReferenceObject());
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


	Quaternionf ModelTransform::getOrientation()
	{
		if(p_ref != NULL)
		{
			return p_ref->getOrientation() * m_rotation;
		}
		return m_rotation;
	}

	Vec3f ModelTransform::getPosition()
	{
		if(p_ref != NULL)
		{
			return p_ref->getPosition() + p_ref->getOrientation() * (m_translation.scale(p_ref->getScale()));
		}
		return m_translation;
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
}