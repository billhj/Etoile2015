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
	class VBOMeshRenderer : public GLMeshRenderer
	{
	public:
		VBOMeshRenderer(const std::string& name = "");
		~VBOMeshRenderer();
		virtual void drawRenderMesh();
		virtual void drawRenderSubMesh(RenderSubMesh* submesh);

		virtual void drawTextureCoord();
		virtual void drawTextureCoordRenderSubMesh(RenderSubMesh* submesh);
	};


	class GPUBasedVBOMeshRenderer : public VBOMeshRenderer
	{
	public:
		GPUBasedVBOMeshRenderer(const std::string& name = "");
		virtual void drawRenderSubMesh(RenderSubMesh* submesh);
	};

	class AvancedGPUBasedVBOMeshRenderer : public GPUBasedVBOMeshRenderer
	{
	public:
		AvancedGPUBasedVBOMeshRenderer(const std::string& name = "");
		virtual void drawRenderSubMesh(RenderSubMesh* submesh);
	};
}
