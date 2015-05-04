/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLMeshRenderPass.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "GLRenderPass.h"
#include "VBOMeshRenderer.h"
#include "ImmediateMeshRenderer.h"

namespace Etoile
{
	enum RenderMode
	{
		RenderMode_Immediate = 0,				///< 			
		RenderMode_GPUVBO,   
		RenderMode_AvancedGPUVBO,         ///< Invalid type
	};

	class GLMeshRenderPass : public GLRenderPass
	{
		GpuProgram * _pGpuCommon;
	public:
		GLMeshRenderPass(const std::string& name): GLRenderPass(name), _pGpuCommon(NULL)
		{
		}

		void setCommonGpuProgram(GpuProgram * gpu)
		{
			_pGpuCommon = gpu;
		}

		virtual void addMesh(Mesh* mesh, RenderMode mode = RenderMode_AvancedGPUVBO)
		{
			MeshRenderer* renderunit;
			if(mode == RenderMode_Immediate)
			{
				renderunit = new ImmediateMeshRenderer(mesh->getName());
				renderunit->setMesh(mesh);
			}
			else if(mode == RenderMode_GPUVBO)
			{
				renderunit = new GPUBasedVBOMeshRenderer(mesh->getName());
				renderunit->setMesh(mesh);
				for(unsigned int j = 0; j < mesh->getSubMeshList().size(); ++j)
				{
					SubMesh* submesh = mesh->getSubMeshList()[j];
					Material* mt = submesh->getMaterial();
					if(_pGpuCommon != NULL)
					{
						mt->setGpuProgram(_pGpuCommon);
					}
				}
			}
			else if(mode == RenderMode_AvancedGPUVBO)
			{
				renderunit = new AvancedGPUBasedVBOMeshRenderer(mesh->getName());
				renderunit->setMesh(mesh);
				for(unsigned int j = 0; j < mesh->getSubMeshList().size(); ++j)
				{
					SubMesh* submesh = mesh->getSubMeshList()[j];
					Material* mt = submesh->getMaterial();
					if(_pGpuCommon != NULL)
					{
						mt->setGpuProgram(_pGpuCommon);
					}
				}
			}
			_renderUnits.push_back(renderunit);
		}

		virtual void addRenderUnit(RenderUnit* ru) override
		{
			MeshRenderer* unit = dynamic_cast<MeshRenderer*>(ru);
			if(unit != NULL)
			{
				Mesh* mesh = unit->getMesh();
				for(unsigned int j = 0; j < mesh->getSubMeshList().size(); ++j)
				{
					SubMesh* submesh = mesh->getSubMeshList()[j];
					Material* mt = submesh->getMaterial();
					if(_pGpuCommon != NULL)
					{
						mt->setGpuProgram(_pGpuCommon);
					}
				}
			}
			_renderUnits.push_back(ru);
		}

		virtual void setMesh(Mesh* mesh, RenderMode mode = RenderMode_AvancedGPUVBO)
		{
			MeshRenderer* renderunit;
			if(mode == RenderMode_Immediate)
			{
				renderunit = new ImmediateMeshRenderer(mesh->getName());
				renderunit->setMesh(mesh);
			}
			else if(mode == RenderMode_GPUVBO)
			{
				renderunit = new GPUBasedVBOMeshRenderer(mesh->getName());
				renderunit->setMesh(mesh);
				for(unsigned int j = 0; j < mesh->getSubMeshList().size(); ++j)
				{
					SubMesh* submesh = mesh->getSubMeshList()[j];
					Material* mt = submesh->getMaterial();
					if(_pGpuCommon != NULL)
					{
						mt->setGpuProgram(_pGpuCommon);
					}
				}
			}
			else if(mode == RenderMode_AvancedGPUVBO)
			{
				renderunit = new AvancedGPUBasedVBOMeshRenderer(mesh->getName());
				renderunit->setMesh(mesh);
				for(unsigned int j = 0; j < mesh->getSubMeshList().size(); ++j)
				{
					SubMesh* submesh = mesh->getSubMeshList()[j];
					Material* mt = submesh->getMaterial();
					if(_pGpuCommon != NULL)
					{
						mt->setGpuProgram(_pGpuCommon);
					}
				}
			}
			_renderUnits.clear();
			_renderUnits.push_back(renderunit);
		}
		virtual void draw() override
		{
			useRenderTargets();
			//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			for(unsigned int i = 0; i < _renderUnits.size(); ++i)
			{
				_renderUnits[i]->draw();
			}
			unUseRenderTargets();
		}

	};
}
