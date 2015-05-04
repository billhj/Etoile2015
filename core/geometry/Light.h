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

namespace Etoile
{

	class Light
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

		void setPosition(const Vec4f& position);
		void setPosition(float x = 1, float y = 1, float z = 1, float w = 0);
		const Vec4f& getPosition() const{ return m_position;}

		void setLookAt(float x = 0, float y = 0, float z = 0, float w = 0);
		const Vec4f& getLookAt() const{return m_lookAt;}
		const Vec4f& getDirectionVector() const{ return m_direction;}

		void setCutOffAngle(float angle);
		float getCutOffAngle()const{return m_cutoff;}

		Matrix4f& getModelViewMatrix();
		Matrix4f& getProjectionMatrix();
		Matrix4f& getModelViewProjectionMatrix();

		void updateMatrix();

		void outputValues(std::ofstream* outfile);
	private:
		void calculateDirection();


	private:

		Vec4f m_diffuse;
		Vec4f m_specular;
		float m_diffuseIntensity;
		float m_specularIntensity;

		Vec4f m_diffuseFinal;
		Vec4f m_specularFinal;

		Vec4f m_position;
		Vec4f m_lookAt;
		Vec4f m_direction;
		float m_cutoff;

		Matrix4f m_modelview, m_projection, m_modelviewProjection;
		int m_id;
		std::string m_name;
	};

}
