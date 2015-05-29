/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file VBORenderSubMesh.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "glhead.h"
#include "geometry/RenderSubMesh.h"


namespace Etoile
{
	class VBO;
	class IndexVBO;
	class VBORenderSubMesh : public RenderSubMesh
	{
	public:
		VBORenderSubMesh(const std::string& name = "");
		~VBORenderSubMesh();
		void buildVBO(GLenum usage/* = GL_STATIC_DRAW_ARB*/);
	
		VBO* p_vertexVBO;
		VBO* p_normalVBO;
		VBO* p_texcoordVBO;
		IndexVBO* p_indexVBO;
		GLenum m_usage;
	};

}
