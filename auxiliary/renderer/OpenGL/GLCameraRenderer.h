/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLCameraRenderer.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "geometry/ObjectRenderer.h"
#include "glhead.h"
#include "geometry/Camera.h"
#include "geometry/ModelTransform.h"
#include "geometry/Frustum.h"

namespace Etoile
{
	class GLCameraRenderer : public ObjectRenderer
	{
		Camera* p_camera;
	public:
		GLCameraRenderer(const std::string& name, Camera* camera = NULL) : ObjectRenderer(name), p_camera(camera)
		{

		}

		GLCameraRenderer(Camera* camera) : ObjectRenderer(camera->getName()), p_camera(camera)
		{

		}

		void setCamera(Camera* camera)
		{
			p_camera = camera;
		}

		virtual void draw()
		{
			Vec3f pos = p_camera->getPosition();
			Frustum* f = p_camera->getFrustum();
			Vec3f nbl = f->m_nbl;
			Vec3f nbr = f->m_nbr;
			Vec3f ntl = f->m_ntl;
			Vec3f ntr = f->m_ntr;
		}


	};

}
