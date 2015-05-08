/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Frustum.cpp
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "math/MathHead.h"
#include "Plane.h"
#include "AxisAlignedBoundingBox.h"

namespace Etoile
{
#define ANG2RAD 3.14159265358979323846/180.0

	enum FrustumTest{OUTSIDE, INTERSECT, INSIDE};
	class Frustum 
	{
	public:
		enum {
			TOP = 0,
			BOTTOM,
			LEFT,
			RIGHT,
			NEARP,
			FARP
		};
	
		/*
		* normal point to the center
		*/
		Plane m_plane[6];

		/*
		* near top left, near top right, near bottom left, near botton right, far top left ....
		*/
		Vec3f m_ntl, m_ntr, m_nbl, m_nbr, m_ftl, m_ftr, m_fbl, m_fbr;
		/*
		* distace to near plane, distance to far plane, width / height, vertical angle, tan of half vertical
		*/
		float m_nearD, m_farD, m_ratio, m_angle, m_tang;

		/*
		* half size
		* near plane width, near plane height, far plane width, far plane height
		*/
		float m_nw, m_nh, m_fw, m_fh;
	public:
		Frustum(){}
		~Frustum(){}

		void setCamInternals(float angle, float ratio, float nearD, float farD);
		/*
		* camera position, target, up vector
		*/
		void setCamDef(Vec3f& p, Vec3f& l, Vec3f& u);

		virtual int pointInFrustum(Vec3f& p) = 0;
		virtual int sphereInFrustum(Vec3f& p, float radius) = 0;
		virtual int boxInFrustum(AxisAlignedBoundingBoxf& b) = 0;

	};

	class FrustumG : public Frustum
	{
		virtual int pointInFrustum(Vec3f& p) override;
		virtual int sphereInFrustum(Vec3f& p, float radius) override;
		virtual int boxInFrustum(AxisAlignedBoundingBoxf& b) override;
	};

}