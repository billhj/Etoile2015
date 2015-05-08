/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Light.cpp
* @brief 
* @date 1/2/2011
*/

#include "Light.h"
#include <cmath>
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

	Light::Light(const std::string& name, int id) : Camera(name), m_id(id)
	{
		setDiffuse(0,0,0,0);
		setSpecular(0,0,0,0);
		setPosition(Vec3f(2,2,2));
		setDiffuseIntensity(1);
		setSpecularIntensity(1);
	}

	Light::Light(Light& light) : Camera(light.getName())
	{
		m_diffuse = light.m_diffuse;
		m_specular = light.m_specular;
		m_diffuseIntensity = light.m_diffuseIntensity;
		m_specularIntensity = light.m_specularIntensity;
		m_diffuseFinal = light.m_diffuseFinal;
		m_specularFinal = light.m_specularFinal;
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

	void Light::outputValues(std::ofstream* outfile)
	{
		(*outfile) << "light: "<< std::endl;
		(*outfile)<<"diffuse: "<<this->m_diffuse.r()<<" "<<this->m_diffuse.g()<<" "<< this->m_diffuse.b()<<" " <<this->m_diffuse.a()<<std::endl;
		(*outfile)<<"specular: "<<this->m_specular.r()<<" "<<this->m_specular.g()<<" "<< this->m_specular.b()<<" " <<this->m_specular.a()<<std::endl;
		(*outfile) << "position; " << this->getPosition().x()<<" " << this->getPosition().y()<<" " << this->getPosition().z()<<" " <<std::endl;

	}

}
