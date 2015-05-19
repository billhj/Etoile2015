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

		void setRenderMesh(RenderMesh* mesh)
		{
			p_mesh = mesh;
		}

		RenderMesh* getRenderMesh(){return p_mesh;}

		void draw()
		{
			drawRenderMesh();
		}

		virtual void drawRenderMesh() = 0;
		virtual void drawTexcoord() = 0;
	protected:
		RenderMesh* p_mesh;
	};

}
