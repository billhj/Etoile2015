/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Plane.h
* @brief 
* @date 1/2/2011
*/

#pragma once

#include "math/MathHead.h"

namespace Etoile
{
	class Plane  
	{

	public:
		//Ax + By + Cz + D = 0
		Vec3f m_normal, m_point;
		float m_d;


		Plane::Plane(Vec3f& v1, Vec3f& v2, Vec3f& v3);
		Plane::Plane(void);
		Plane::~Plane();

		void set3Points(Vec3f& v1,  Vec3f& v2,  Vec3f& v3);
		void setNormalAndPoint(Vec3f& normal, Vec3f& point);
		void setCoefficients(float a, float b, float c, float d);

		//dist = A*rx + B*ry + C*rz + D = n . r  + D
		float distance(Vec3f& p);

		/***
		**	Projecting a point to a plane
		*	p = q - dist * n
		*/

	};
}

