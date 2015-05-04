/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Light.cpp
* @brief 
* @date 1/2/2011
*/

#include "Light.h"
#include <cmath>

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

	Light::Light(const std::string& name, int id) : m_name(name), m_id(id)
	{
		setDiffuse(0,0,0,0);
		setSpecular(0,0,0,0);
		setPosition(0,0,0,0);
		setDiffuseIntensity(1);
		setSpecularIntensity(1);
		setLookAt(0,0,0,1);
		m_cutoff = 60;
	}

	Light::Light(Light& light)
	{
		 m_name = light.m_name;
		 m_diffuse = light.m_diffuse;
		 m_specular = light.m_specular;
		 m_diffuseIntensity = light.m_diffuseIntensity;
		 m_specularIntensity = light.m_specularIntensity;
		 m_diffuseFinal = light.m_diffuseFinal;
		 m_specularFinal = light.m_specularFinal;
		 m_position = light.m_position;
		 m_lookAt = light.m_lookAt;
		 m_direction = light.m_direction;
		 m_cutoff = light.m_cutoff;
		m_modelview = light.m_modelview;
		m_projection = light.m_projection;
		m_modelviewProjection = light.m_modelviewProjection;
		m_id = light.m_id;
	}

	void Light::setId(int id)
	{
		m_id = id;
	}

	int Light::getId()
	{
		return m_id;
	}

	void Light::setLookAt(float x, float y, float z, float w)
	{
		this->m_lookAt[0] = x;
		this->m_lookAt[1] = y;
		this->m_lookAt[2] = z;
		this->m_lookAt[3] = w;
		calculateDirection();
	}

	void Light::setCutOffAngle(float angle)
	{
		m_cutoff = angle;
	}

	void Light::calculateDirection()
	{
		float length = sqrt( (m_lookAt[0]-m_position[0]) * (m_lookAt[0]-m_position[0])
			+  (m_lookAt[1]-m_position[1]) * (m_lookAt[1]-m_position[1])
			+  (m_lookAt[2]-m_position[2]) * (m_lookAt[2]-m_position[2])
			);
		m_direction[0] = (m_lookAt[0]-m_position[0]) / length;
		m_direction[1] = (m_lookAt[1]-m_position[1]) / length;
		m_direction[2] = (m_lookAt[2]-m_position[2]) / length;
		m_direction[3] = 0;
	}



	void Light::setDiffuse(const Vec4f& diffuse)
	{
		m_diffuse = diffuse;
		updateDiffuse();
	}


	void Light::setSpecular(const Vec4f& specular)
	{
		m_specular = specular;
		updateSpecular();

	}

	void Light::setDiffuse(float r, float g, float b, float a)
	{
		this->m_diffuse[0] = r;
		this->m_diffuse[1] = g;
		this->m_diffuse[2] = b;
		this->m_diffuse[3] = a;
		updateDiffuse();

	}
	void Light::setSpecular(float r, float g, float b, float a)
	{
		this->m_specular[0] = r;
		this->m_specular[1] = g;
		this->m_specular[2] = b;
		this->m_specular[3] = a;
		updateSpecular();
	}

	void Light::setDiffuseIntensity(float id)
	{
		this->m_diffuseIntensity = id;
		updateDiffuse();

	}
	void Light::setSpecularIntensity(float is)
	{
		this->m_specularIntensity = is;
		updateSpecular();
	}

	void Light::updateDiffuse()
	{
		m_diffuseFinal = m_diffuse * m_diffuseIntensity;
	}
	void Light::updateSpecular()
	{
		m_specularFinal = m_specular * m_specularIntensity;
	}


	void Light::setPosition(const Vec4f& position)
	{
		m_position = position;
		calculateDirection();
	}

	void Light::setPosition(float x, float y, float z, float w)
	{
		this->m_position[0] = x;
		this->m_position[1] = y;
		this->m_position[2] = z;
		this->m_position[3] = w;
		calculateDirection();
	}

	Matrix4f& Light::getModelViewMatrix()
	{
		return m_modelview;
	}

	Matrix4f& Light::getProjectionMatrix()
	{
		return m_projection;
	}

	Matrix4f& Light::getModelViewProjectionMatrix()
	{
		return m_modelviewProjection;
	}

	void Light::updateMatrix()
	{
		m_modelviewProjection = m_projection * m_modelview;

	}

	void Light::outputValues(std::ofstream* outfile)
	{
		(*outfile) << "light: "<< std::endl;
		(*outfile)<<"diffuse: "<<this->m_diffuse.r()<<" "<<this->m_diffuse.g()<<" "<< this->m_diffuse.b()<<" " <<this->m_diffuse.a()<<std::endl;
		(*outfile)<<"specular: "<<this->m_specular.r()<<" "<<this->m_specular.g()<<" "<< this->m_specular.b()<<" " <<this->m_specular.a()<<std::endl;
		(*outfile) << "position; " << this->getPosition().x()<<" " << this->getPosition().y()<<" " << this->getPosition().z()<<" " << this->getPosition().w()<<std::endl;

	}

}
