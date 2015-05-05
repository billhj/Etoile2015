/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file VBOMeshRenderer.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "GLMeshRenderer.h"
#include "VBO.h"
#include "IndexVBO.h"

namespace Etoile
{
	struct SubMeshVBOUnit
	{
		VBOUnit m_vertexVBO;
		VBOUnit m_normalVBO;
		VBOUnit m_texCoordVBO;
		IndexVBO* p_indexVBO;
		SubMeshVBOUnit()
		{
			m_vertexVBO._pVBO = NULL;
			m_normalVBO._pVBO = NULL;
			m_texCoordVBO._pVBO = NULL;
			p_indexVBO = NULL;
		}
		~SubMeshVBOUnit()
		{
			if(p_indexVBO != NULL)
				delete p_indexVBO;
		}
	};

	class VBOMeshRenderer : public GLMeshRenderer
	{
	public:
		VBOMeshRenderer(const std::string& name = "");
		~VBOMeshRenderer();
		virtual void drawSubMesh(SubMesh* submesh, int idx);
		virtual void setMesh(Mesh* mesh) override;
		//virtual void updateVBO(Mesh* mesh);
		//virtual void updateVBO(SubMesh* submesh, int idx);
	protected:
		int getVBOUnitIndexByName(const std::string& name);
		virtual void createVBO(Mesh* mesh, GLenum usage = GL_STATIC_DRAW_ARB);
		std::vector<SubMeshVBOUnit*> _vboUnitList;
		std::map<std::string, int> _vboUnitIndex;
	};


	class GPUBasedVBOMeshRenderer : public VBOMeshRenderer
	{
	public:
		GPUBasedVBOMeshRenderer(const std::string& name = "");
		virtual void drawSubMesh(SubMesh* submesh, int idx);
	};

	class AvancedGPUBasedVBOMeshRenderer : public GPUBasedVBOMeshRenderer
	{
	public:
		AvancedGPUBasedVBOMeshRenderer(const std::string& name = "");
		virtual void drawSubMesh(SubMesh* submesh, int idx);
	};
}
