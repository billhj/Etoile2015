/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Material.h
* @brief 
* @date 1/2/2011
*/

#include "Material.h"
#include <string>
#include "MaterialManager.h"

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

	Material::Material(const std::string& name): m_name(name), m_ka(1), m_kd(1), m_ks(1), m_shininess(64)
	{
		
		m_ambient = Vec4f(0.2f,0.2f,0.2f,1);
		m_diffuse = Vec4f(1,1,1,1);
		m_specular = Vec4f(0.2f,0.2f,0.2f,1);
		m_shininess = 64;

		m_diffuseFinal = m_diffuse * m_kd;
		m_specularFinal = m_specular * m_ks;
		m_ambientFinal = m_ambient * m_ka;

		m_transparency = 0;
		m_dissolved_opacity = 1;

		p_gpuProgram = NULL;
		/*setDiffuseTexture("emptyMap");
		setSpecularTexture("emptyMap");
		setBumpMap("emptyMap");*/
		MaterialManager::getInstance()->addMaterial(this);
		
	}

	Material::Material(Material& m)
	{
		m_name = m.m_name + "_copy";
		m_ambient = m.m_ambient;
		m_diffuse = m.m_diffuse;
		m_specular = m.m_diffuse;
		m_ka = m.m_ka;
		m_kd = m.m_kd;
		m_ks = m.m_ks;

		m_transparency = m.m_transparency;
		m_dissolved_opacity = m.m_dissolved_opacity;
		m_ambientFinal = m.m_ambientFinal;
		m_diffuseFinal = m.m_diffuseFinal;
		m_specularFinal = m.m_specularFinal;

		m_shininess = m.m_shininess;
		m_textures = m.m_textures;
		p_gpuProgram = m.p_gpuProgram;
		MaterialManager::getInstance()->addMaterial(this);
	}

	void Material::setAmbient(const Vec4f& ambient)
	{
		m_ambient = ambient;
		updateAmbient();
	}


	void Material::setDiffuse(const Vec4f& diffuse)
	{
		m_diffuse = diffuse;
		updateDiffuse();
	}


	void Material::setSpecular(const Vec4f& specular)
	{
		m_specular = specular;
		updateSpecular();
	}

	void Material::setAmbient(float r, float g, float b, float a)
	{
		this->m_ambient[0] = r;
		this->m_ambient[1] = g;
		this->m_ambient[2] = b;
		this->m_ambient[3] = a;
		updateAmbient();

	}
	void Material::setDiffuse(float r, float g, float b, float a)
	{
		this->m_diffuse[0] = r;
		this->m_diffuse[1] = g;
		this->m_diffuse[2] = b;
		this->m_diffuse[3] = a;
		updateDiffuse();

	}
	void Material::setSpecular(float r, float g, float b, float a)
	{
		this->m_specular[0] = r;
		this->m_specular[1] = g;
		this->m_specular[2] = b;
		this->m_specular[3] = a;
		updateSpecular();

	}


	void Material::updateDiffuse()
	{
		m_diffuseFinal = m_diffuse * m_kd;
	}

	void Material::updateSpecular()
	{
		m_specularFinal = m_specular * m_ks;
	}
	void Material::updateAmbient()
	{
		m_ambientFinal = m_ambient * m_ka;
	}



	void  Material::setDiffuseTexture(Texture* t)
	{
		m_textures["diffuseTexture"] =  t;
	}


	Texture* Material::getDiffuseTexture() const
	{

		std::map<std::string, Texture*>::const_iterator itor = m_textures.find("diffuseTexture");
		if( itor != m_textures.end())
		{
			return itor->second;
		}
		return NULL;
	}


	void  Material::setSpecularTexture(Texture* t)
	{
		m_textures["specularTexture"] =  t;
	}



	Texture* Material::getSpecularTexture() const
	{
		std::map<std::string, Texture*>::const_iterator itor = m_textures.find("specularTexture");
		if(itor != m_textures.end())
		{
			return itor->second;
		}
		return NULL;
	}

	void Material::setBumpMap(Texture* t){
		m_textures["bumpMap"] =  t;
	}

	Texture* Material::getBumpMap() const
	{
		std::map<std::string, Texture*>::const_iterator itor = m_textures.find("bumpMap");
		if(itor != m_textures.end())
		{
			return itor->second;
		}
		return NULL;
	}

	void Material::outputValues(std::ofstream* outfile)
	{
		(*outfile) << "Material: "<<this->m_name << std::endl;
		(*outfile) << "ambient; " << this->m_ambient[0]<<" " << this->m_ambient[1]<<" " << this->m_ambient[2]<<" " << this->m_ambient[3]<<std::endl;
		(*outfile)<<"diffuse: "<<this->m_diffuse[0]<<" "<<this->m_diffuse[1]<<" "<< this->m_diffuse[2]<<" " <<this->m_diffuse[3]<<std::endl;
		(*outfile)<<"specular: "<<this->m_specular[0]<<" "<<this->m_specular[1]<<" "<< this->m_specular[2]<<" " <<this->m_specular[3]<<std::endl;
		(*outfile)<<"shininess: "<<this->m_shininess<<std::endl;

	}
}
