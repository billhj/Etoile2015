/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file VBORenderSubMesh.cpp
* @brief 
* @date 1/2/2011
*/

#include "VBORenderSubMesh.h"
#include "GLSLGpuProgram.h"

namespace Etoile
{
	VBORenderSubMesh::VBORenderSubMesh(GLenum usage, const std::string& name) : m_usage(usage), RenderSubMesh(name)
	{
		p_vertexVBO = NULL;
		p_normalVBO = NULL;
		p_texcoordVBO = NULL;
		p_indexVBO = NULL;
	}

	VBORenderSubMesh::~VBORenderSubMesh()
	{
		if(NULL != p_vertexVBO)
		{
			delete p_vertexVBO;
		}

		if(NULL != p_normalVBO)
		{
			delete p_normalVBO;
		}

		if(NULL != p_texcoordVBO)
		{
			delete p_texcoordVBO;
		}

		if(NULL != p_indexVBO)
		{
			delete p_indexVBO;
		}
	}

	
	void VBORenderSubMesh::buildVBO()
	{
		size_t sizeComponent = m_vdata.size();
		size_t sizeTextureCord = m_tdata.size();
		p_normalVBO = new VBO(sizeComponent * 3, &(m_ndata[0][0]), m_usage);
		p_texcoordVBO = new VBO(sizeTextureCord * 2, &(m_tdata[0][0]), m_usage);
		p_vertexVBO = new VBO(sizeComponent * 3, &(m_vdata[0][0]), m_usage);
	    p_indexVBO = new IndexVBO(m_vertices_index_face.size(), &(m_vertices_index_face[0]), m_usage);
	}

}
