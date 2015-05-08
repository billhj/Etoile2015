/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Light.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <fstream>
#include "math/Matrix.h"
#include "math/Vectors.h"
#include "Camera.h"

namespace Etoile
{

	class Light : public Camera
	{
	public:
		Light(const std::string& name = "", int id = 0);
		Light(Light& light);

		void setId(int id);
		int getId();
		void setDiffuse(const Vec4f& diffuse);
		void setSpecular(const Vec4f& specular);
		void setDiffuse(float r = 1, float g = 1, float b = 1, float a = 1);
		void setSpecular(float r = 1, float g = 1, float b = 1, float a = 1);
		void updateDiffuse();
		void updateSpecular();

		const Vec4f& getDiffuseFinal() const{ return m_diffuseFinal;}
		const Vec4f& getSpecularFinal() const{ return m_specularFinal;}

		const Vec4f& getDiffuse() const{ return m_diffuse;}
		const Vec4f& getSpecular() const{ return m_specular;}

		void setDiffuseIntensity(float id = 1);
		void setSpecularIntensity(float is = 1);
		float getDiffuseIntensity()const{return m_diffuseIntensity;}
		float getSpecularIntensity()const{return m_specularIntensity;}

		void outputValues(std::ofstream* outfile);

	private:

		Vec4f m_diffuse;
		Vec4f m_specular;
		float m_diffuseIntensity;
		float m_specularIntensity;

		Vec4f m_diffuseFinal;
		Vec4f m_specularFinal;
		int m_id;
	};

}
