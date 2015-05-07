/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file PickingRenderer.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "GLMeshRenderer.h"
#include "VBO.h"
#include "IndexVBO.h"

namespace Etoile
{
	struct RenderSubMeshVBOUnit
	{
		VBOUnit m_vertexVBO;
		VBOUnit m_normalVBO;
		VBOUnit m_texCoordVBO;
		IndexVBO* p_indexVBO;
		RenderSubMeshVBOUnit()
		{
			m_vertexVBO._pVBO = NULL;
			m_normalVBO._pVBO = NULL;
			m_texCoordVBO._pVBO = NULL;
			p_indexVBO = NULL;
		}
		~RenderSubMeshVBOUnit()
		{
			if(p_indexVBO != NULL)
				delete p_indexVBO;
		}
	};

	class PickingRenderer : public GLMeshRenderer
	{
	public:
		PickingRenderer(const std::string& name = "");
		~PickingRenderer();
		virtual void drawRenderMesh();
		virtual void drawRenderSubMesh(RenderSubMesh* submesh, int idx);
		virtual void setRenderMesh(RenderMesh* mesh) override;
		
		void processPickUbyte();
		void processPickInt();
		void processPickFloat();
	protected:
		int getVBOUnitIndexByName(const std::string& name);
		virtual void createVBO(RenderMesh* mesh, GLenum usage = GL_STATIC_DRAW_ARB);
		std::vector<RenderSubMeshVBOUnit*> _vboUnitList;
		std::map<std::string, int> _vboUnitIndex;
		int m_cursorX, m_cursorY;
	};


}
