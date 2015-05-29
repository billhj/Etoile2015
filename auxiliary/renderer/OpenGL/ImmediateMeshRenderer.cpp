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

	void ImmediateMeshRenderer::drawTexcoord()
	{
		drawMeshTexcoord();
	}


	void ImmediateMeshRenderer::drawMeshTexcoord()
	{
		if(p_mesh == NULL) return;
		Matrix4f modelM;
		ModelTransform* t = this->getEntity()->getTransformation();
		useTransform(t);
		const std::vector<RenderSubMesh*>& submeshlist = p_mesh->getRenderSubMeshList();

		for(unsigned int i = 0; i < submeshlist.size(); ++i)
		{
			RenderSubMesh* submesh = submeshlist[i];
			drawSubMeshTexcoord(submesh);
		}
		drawAABB();
		unUseTransform(t);
	}

	void ImmediateMeshRenderer::drawSubMeshTexcoord(RenderSubMesh* submesh)
	{
		const std::vector<Vec3f>& vertices = submesh->m_vdata;
		const std::vector<Vec3f>& texs = submesh->m_tdata;

		if(texs.size() < 1 || vertices.size() < 1) return;

		const std::vector<int>& faceIndices = submesh->m_vertices_index_face;

		glBegin(GL_TRIANGLES);
		for(unsigned int i = 0; i < faceIndices.size(); ++i)
		{
			glColor3fv( &texs[faceIndices[i]][0] );
			glVertex3fv( &vertices[faceIndices[i]][0] );
		}
		glEnd();
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
