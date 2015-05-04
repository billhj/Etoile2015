/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Material.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <fstream>
#include "math/Vectors.h"
#include <map>
#include "Texture.h"

#ifdef USING_BOOST
#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#endif

namespace Etoile
{
	class GpuProgram;
	class Material
	{
	public:
		Material(const std::string& name);
		Material(Material&);

		void setAmbient(const Vec4f& ambient);
		void setDiffuse(const Vec4f& diffuse);
		void setSpecular(const Vec4f& specular);
		void setAmbient(float r, float g, float b, float a);
		void setDiffuse(float r, float g, float b, float a);
		void setSpecular(float r, float g, float b, float a);
		void setSpecularShininess(int shininess){ this->m_shininess = shininess; }

		void setTransparency(float transparencyValue){ m_transparency = transparencyValue;}
		float getTransparency(){ return m_transparency;}
		void setDissolvedOpacity(float dissolved){ m_dissolved_opacity = dissolved;}
		float getDissolvedOpacity(){return m_dissolved_opacity;}

		void updateDiffuse();
		void updateSpecular();
		void updateAmbient();


		const Vec4f& getAmbient()const{ return m_ambient;}
		const Vec4f& getDiffuse()const{ return m_diffuse;}
		const Vec4f& getSpecular()const{ return m_specular;}

		const Vec4f& getAmbientFinal()const{ return m_ambientFinal;}
		const Vec4f& getDiffuseFinal()const{ return m_diffuseFinal;}
		const Vec4f& getSpecularFinal()const{ return m_specularFinal;}

		int getSpecularShininess()const{return m_shininess;}


		void setAmbientConstant(float ka){this->m_ka=ka; updateAmbient(); }
		void setDiffuseConstant(float kd){this->m_kd=kd; updateDiffuse(); }
		void setSpecularConstant(float ks){this->m_ks=ks; updateSpecular(); }
		float getAmbientConstant() const { return m_ka;}
		float getDiffuseConstant() const { return m_kd;}
		float getSpecularConstant() const { return m_ks;}

		void setDiffuseTexture(Texture*);
		Texture* getDiffuseTexture() const;


		void setSpecularTexture(Texture*);
		Texture* getSpecularTexture() const;

		void setBumpMap(Texture*);
		Texture* getBumpMap() const;
		
		void setTexture(const std::string& bindingName, Texture* t)
		{
			m_textures[bindingName] = t;
		}

		void removeTexture(Texture* t)
		{
			std::map<std::string, Texture*>::iterator itor = m_textures.begin();
			for(; itor != m_textures.end(); ++itor)
			{
				if(itor->second == t)
				{
					m_textures.erase(itor);
				}
			}
		}

		void removeTexture(const std::string& name)
		{
			std::map<std::string, Texture*>::iterator itor = m_textures.find(name);
			if(itor != m_textures.end())
			{
				m_textures.erase(itor);
			}
		}

		std::map<std::string, Texture*>& getTextures(){ return m_textures;}

		void setGpuProgram(GpuProgram* gpuProgram)
		{
			p_gpuProgram = gpuProgram;
		}

		GpuProgram* getGpuProgram() const
		{
			return p_gpuProgram;
		}

		void outputValues(std::ofstream* outfile);
		const std::string getName(){return m_name;}
		void setName(const std::string& name){ m_name = name;}

#ifdef USING_BOOST
		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & _materialname;
			ar & m_ambient;
			ar & m_diffuse;
			ar & m_specular;
			ar & m_ka;
			ar & m_kd;
			ar & m_ks;

			ar & m_transparency;
			ar & m_dissolved_opacity;
			ar & m_ambientFinal;
			ar & m_diffuseFinal;
			ar & m_specularFinal;

			ar & m_shininess;
			ar & m_textures;
			ar & p_gpuProgram;
			ar & _materialname;
		}
#endif

	private:

		Vec4f m_ambient;
		Vec4f m_diffuse;
		Vec4f m_specular;
		float m_ka;
		float m_kd;
		float m_ks;

		float m_transparency;
		float m_dissolved_opacity;
		Vec4f m_ambientFinal;
		Vec4f m_diffuseFinal;
		Vec4f m_specularFinal;

		int m_shininess;
		std::map<std::string, Texture*> m_textures;
		GpuProgram* p_gpuProgram;
		std::string m_name;
	};

	//typedef std::map<std::string, Material*> MaterialList;
}
