/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file RenderMeshRenderer.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "geometry/ObjectRenderer.h"
#include "geometry/RenderMesh.h"
#include "geometry/ModelTransform.h"

namespace Etoile
{
	class MeshRenderer : public ObjectRenderer
	{
	public:
		MeshRenderer(const std::string& name): ObjectRenderer(name), p_mesh(NULL)
		{
		}
		virtual void setRenderMesh(RenderMesh* mesh, ModelTransform* transform)
		{
			p_mesh = mesh;
			p_transform = transform;
		}

		virtual void setRenderMesh(RenderMesh* mesh)
		{
			p_mesh = mesh;
		}

		virtual void seTransform(ModelTransform* transform)
		{
			p_transform = transform;
		}

		virtual RenderMesh* getRenderMesh(){return p_mesh;}

		void draw()
		{
			drawRenderMesh();
		}

		virtual void drawRenderMesh() = 0;
		
	protected:
		RenderMesh* p_mesh;
		ModelTransform* p_transform;
	};

}
