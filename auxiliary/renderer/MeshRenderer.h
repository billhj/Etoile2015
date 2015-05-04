/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MeshRenderer.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "geometry/ObjectRenderer.h"
#include "geometry/Mesh.h"
#include "geometry/ModelTransform.h"

namespace Etoile
{
	class MeshRenderer : public ObjectRenderer
	{
	public:
		MeshRenderer(const std::string& name): ObjectRenderer(name), p_mesh(NULL)
		{
		}
		virtual void setMesh(Mesh* mesh, ModelTransform* transform)
		{
			p_mesh = mesh;
			p_transform = transform;
		}

		virtual void setMesh(Mesh* mesh)
		{
			p_mesh = mesh;
		}

		virtual void seTransform(ModelTransform* transform)
		{
			p_transform = transform;
		}

		virtual Mesh* getMesh(){return p_mesh;}

		void draw()
		{
			drawMesh();
		}

		virtual void drawMesh() = 0;
		
	protected:
		Mesh* p_mesh;
		ModelTransform* p_transform;
	};

}
