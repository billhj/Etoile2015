/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ModelTransform.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "math/MathHead.h"
#include "AxisAlignedBoundingBox.h"
#include "Component.h"

namespace Etoile
{
	class ModelTransform : public Component
	{
	public:
		ModelTransform();
		ModelTransform(const ModelTransform& obj);
		ModelTransform(const Vec3f& translate, const Quaternionf& rotation, const Vec3f& scale = Vec3f(1,1,1), ModelTransform* ref = NULL);
		void set(const Vec3f& translate, const Quaternionf& rotation, const Vec3f& scale = Vec3f(1,1,1), ModelTransform* ref = NULL);
		ModelTransform& operator=(const ModelTransform& obj);
		void setReferenceObject(ModelTransform* obj);
		ModelTransform* getReferenceObject() const {return p_ref;}

		void setScale(const Vec3f& scale);
		Vec3f getScale() const{return m_scale;}
		void setTranslation(const Vec3f& translation);
		Vec3f getTranslation() const {return m_translation;}
		void translate(const Vec3f& translation);
		void setRotation(const Quaternionf& rotation);
		Quaternionf getRotation() const {return m_rotation;}
		void rotate(const Quaternionf& rotation);
		Matrix4f getModelMatrix();
		Matrix4f getGLModelMatrix();
		void setTransform(const Vec3f& scale, const Quaternionf& rotate, const Vec3f& translate)
		{
			m_scale = scale, m_rotation = rotate, m_translation = translate;
			m_rotation.normalize();
			m_modelTransform = Matrix4f().makeTranslation(m_translation) * Matrix4f().makeAxisRotation(m_rotation.axis(), m_rotation.angle()) * Matrix4f().makeScale(m_scale);
		}

		void setTransform()
		{
			setTransform(m_scale, m_rotation, m_translation);
		}

		Quaternionf getOrientation();
		Vec3f getPosition();

		void setOrientation(const Quaternionf& orientation);
		void setPosition(const Vec3f& pos);
	protected:
		Vec3f m_scale; //local
		Vec3f m_translation;  //local
		Quaternionf m_rotation;  //local
		Matrix4f m_modelTransform;
		ModelTransform* p_ref;
	};

}