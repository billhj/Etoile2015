/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file VBORenderSubMesh.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "geometry/RenderSubMesh.h"
#include "VBO.h"
#include "IndexVBO.h"

namespace Etoile
{

	class VBORenderSubMesh : public RenderSubMesh
	{
	public:
		VBORenderSubMesh(GLenum usage = GL_STATIC_DRAW_ARB, const std::string& name = "");
		~VBORenderSubMesh();
		void buildVBO();
	
		VBO* p_vertexVBO;
		VBO* p_normalVBO;
		VBO* p_texcoordVBO;
		IndexVBO* p_indexVBO;
		GLenum m_usage;

		VBO* p_texcoordColorVBO;
	};

}
