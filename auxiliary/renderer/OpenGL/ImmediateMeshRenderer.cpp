/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ImmediateMeshRenderer.cpp
* @brief 
* @date 1/2/2011
*/

#include "ImmediateMeshRenderer.h"

namespace Etoile
{
	ImmediateMeshRenderer::ImmediateMeshRenderer(const std::string& name) : GLMeshRenderer(name)
	{
	}


	void ImmediateMeshRenderer::draw()
	{
		drawRenderMesh();
	}

	DisplayListMeshRenderer::DisplayListMeshRenderer(const std::string& name) : GLMeshRenderer(name)
	{
	}


	void DisplayListMeshRenderer::draw()
	{
		glCallList(_calllist);
	}


	void DisplayListMeshRenderer::setRenderMesh(RenderMesh* mesh)
	{
		p_mesh = mesh;
		generateCallList();
	}

	void DisplayListMeshRenderer::generateCallList()
	{
		_calllist = glGenLists(1);
		glNewList(_calllist, GL_COMPILE);
		drawRenderMesh();
		glEndList();
	}

}
