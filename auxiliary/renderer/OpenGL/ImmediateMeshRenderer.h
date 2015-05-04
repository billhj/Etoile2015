/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MeshRenderPass.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "glhead.h"
#include "GLMeshRenderer.h"

namespace Etoile
{
	class ImmediateMeshRenderer : public GLMeshRenderer
	{
	public:
		ImmediateMeshRenderer(const std::string& name);
		virtual void draw() override;
	};


	class DisplayListMeshRenderer : public GLMeshRenderer
	{
	public:
		DisplayListMeshRenderer(const std::string& name);
		~DisplayListMeshRenderer(){  if(glIsList(_calllist)){glDeleteLists(_calllist, 1);} }
		virtual void draw() override;
		virtual void setMesh(Mesh* mesh);
	protected:
		void generateCallList();
		GLuint _calllist;
	};

}
