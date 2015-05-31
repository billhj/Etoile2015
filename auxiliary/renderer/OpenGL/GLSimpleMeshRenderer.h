/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLSimpleMeshRenderer.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "geometry/ObjectRenderer.h"
#include "geometry/SimpleMesh.h"

#ifdef USING_GLEW
	#include "VBO.h"
	#include "IndexVBO.h"
#endif

namespace Etoile
{
	class GLSimpleMeshRenderer : public ObjectRenderer
	{
		SimpleMesh* p_mesh;
	public:
		GLSimpleMeshRenderer(const std::string& name = "");
		~GLSimpleMeshRenderer();
		void setSimpleMesh(SimpleMesh* mesh);
		void draw();
#ifdef USING_GLEW
		void setSimpleMesh(SimpleMesh* mesh, GLenum usage = GL_STATIC_DRAW_ARB);
		void drawSimpleMesh(SimpleMesh* mesh);
		VBO* p_vertexVBO;
		VBO* p_normalVBO;
		VBO* p_texcoordVBO;
		IndexVBO* p_indexVBO;
#else

#endif

	};

}