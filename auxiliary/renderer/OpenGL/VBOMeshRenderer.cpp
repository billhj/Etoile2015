/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file VBOMeshRenderer.cpp
* @brief 
* @date 1/2/2011
*/

#include "VBOMeshRenderer.h"
#include "GLSLGpuProgram.h"
#include "VBORenderSubMesh.h"

namespace Etoile
{
	VBOMeshRenderer::VBOMeshRenderer(const std::string& name) : GLMeshRenderer(name)
	{
	}

	VBOMeshRenderer::~VBOMeshRenderer()
	{

	}

	void VBOMeshRenderer::drawRenderMesh()
	{
		if(p_mesh == NULL) return;
		Matrix4f modelM;
		ModelTransform* t = this->getEntity()->getTransformation();
		useTransform(t);
		const std::vector<RenderSubMesh*>& submeshlist = p_mesh->getRenderSubMeshList();

		for(unsigned int i = 0; i < submeshlist.size(); ++i)
		{
			RenderSubMesh* submesh = submeshlist[i];
			drawRenderSubMesh(submesh);
		}

		drawAABB();
		unUseTransform(t);
	}

	void VBOMeshRenderer::drawRenderSubMesh(RenderSubMesh* submesh)
	{
		VBORenderSubMesh* vbosubmesh = dynamic_cast<VBORenderSubMesh*>(submesh);
		if(NULL == vbosubmesh) return;

		glEnable( GL_TEXTURE_2D );
		Material& material = vbosubmesh->getMaterial();

		applyMaterial(&material);

		/*Matrix4f modelM = submesh->getGLModelMatrix() * p_mesh->getGLModelMatrix();
		glPushMatrix();
		glLoadMatrixf(&modelM[0][0]);*/

		Texture* t = material.getDiffuseTexture();
		if(t != NULL)
		{
			t->use();
		}

		vbosubmesh->p_texcoordVBO->use();
		glTexCoordPointer(2, GL_FLOAT, 0, 0);
		vbosubmesh->p_normalVBO->use();
		glNormalPointer(GL_FLOAT, 0, 0);
		vbosubmesh->p_vertexVBO->use();
		glVertexPointer(3, GL_FLOAT, 0, 0);

		printOpenGLError();

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);

		printOpenGLError();

		vbosubmesh->p_indexVBO->use();
		glDrawElements( GL_TRIANGLES, vbosubmesh->p_indexVBO->getSize(), GL_UNSIGNED_INT, 0 );
		vbosubmesh->p_indexVBO->unUse();

		printOpenGLError();

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);


		vbosubmesh->p_texcoordVBO->unUse();
		vbosubmesh->p_normalVBO->unUse();
		vbosubmesh->p_vertexVBO->unUse();

		printOpenGLError();

		if(t != NULL)
		{
			t->unUse();
		}
		//glPopMatrix();


	}

	void VBOMeshRenderer::drawMeshTexcoord()
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

	void VBOMeshRenderer::drawSubMeshTexcoord(RenderSubMesh* submesh)
	{
		VBORenderSubMesh* vbosubmesh = dynamic_cast<VBORenderSubMesh*>(submesh);

		if(submesh->getTextureCoordsColor().size() < 1 || submesh->getVertices().size() < 1) return;

		if(NULL == vbosubmesh) return;
		vbosubmesh->p_texcoordColorVBO->use();
		glColorPointer(3, GL_FLOAT, 0, 0);
		vbosubmesh->p_vertexVBO->use();
		glVertexPointer(3, GL_FLOAT, 0, 0);

		printOpenGLError();

		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);

		printOpenGLError();

		vbosubmesh->p_indexVBO->use();
		glDrawElements(GL_TRIANGLES, vbosubmesh->p_indexVBO->getSize(), GL_UNSIGNED_INT, 0 );
		vbosubmesh->p_indexVBO->unUse();

		printOpenGLError();
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);


		vbosubmesh->p_texcoordColorVBO->unUse();
		vbosubmesh->p_vertexVBO->unUse();

		printOpenGLError();

	}

	/*void VBOMeshRenderer::createVBO(RenderMesh* mesh, GLenum usage)
	{
	const std::vector<RenderSubMesh*>& submeshlist = mesh->getRenderSubMeshList();

	for(unsigned int i = 0; i < submeshlist.size(); ++i)
	{
	RenderSubMesh* submesh = submeshlist[i];

	RenderSubMeshVBOUnit* info = new RenderSubMeshVBOUnit();
	size_t sizeComponent = submesh->getVertices().size();
	size_t sizeTextureCord = submesh->getTextureCoords().size();

	VBO* normalVBO = new VBO(sizeComponent * 3, &(submesh->getNormals()[0][0]), usage);
	info->m_normalVBO._pVBO = normalVBO;
	info->m_normalVBO._attributeName = "In_Normal";
	info->m_normalVBO._numberComponents = 3;
	info->m_normalVBO._primitive = GL_TRIANGLES;

	VBO* texCoordVBO = new VBO(sizeTextureCord * 2, &(submesh->getTextureCoords()[0][0]), usage);
	info->m_texCoordVBO._pVBO = texCoordVBO;
	info->m_texCoordVBO._attributeName = "In_TextureCoord";
	info->m_texCoordVBO._numberComponents = 2;
	info->m_texCoordVBO._primitive = GL_TRIANGLES;

	VBO* vertexVBO = new VBO(sizeComponent * 3, &(submesh->getVertices()[0][0]), usage);
	info->m_vertexVBO._pVBO = vertexVBO;
	info->m_vertexVBO._attributeName = "In_Vertex";
	info->m_vertexVBO._numberComponents = 3;
	info->m_vertexVBO._primitive = GL_TRIANGLES;


	IndexVBO* p_indexVBO = new IndexVBO(submesh->getVertexIndexForFaces().size(), &(submesh->getVertexIndexForFaces()[0]), usage);
	info->p_indexVBO = p_indexVBO;
	_vboUnitList.push_back(info);

	}
	}*/


	GPUBasedVBOMeshRenderer::GPUBasedVBOMeshRenderer(const std::string& name) : VBOMeshRenderer(name)
	{}


	void GPUBasedVBOMeshRenderer::drawRenderSubMesh(RenderSubMesh* submesh)
	{
		VBORenderSubMesh* vbosubmesh = dynamic_cast<VBORenderSubMesh*>(submesh);
		if(NULL == vbosubmesh) return;

		glEnable( GL_TEXTURE_2D );

		Material& material = submesh->getMaterial();

		applyMaterial(&material);
		GLSLGpuProgram* gpuprogram = (GLSLGpuProgram*)material.getGpuProgram();

		Matrix4f modelM;
		ModelTransform* t = this->getEntity()->getTransformation();
		if(t)
		{
			modelM = t->getGLModelMatrix();
		}
		if(gpuprogram != NULL)
		{
			gpuprogram->setUniformVariable("In_WorldMatrix",  modelM);
			std::map<std::string, Texture*>& idxs = material.getTextures();
			std::map<std::string, Texture*>::iterator itor;

			for(itor = idxs.begin(); itor != idxs.end(); ++itor)
			{
				std::string bName = itor->first;
				Texture* t = itor->second;
				gpuprogram->bindTexture(bName, t);
			}
			gpuprogram->drawIndexVBO(GL_TRIANGLES, vbosubmesh->p_vertexVBO, vbosubmesh->p_normalVBO, vbosubmesh->p_texcoordVBO, vbosubmesh->p_indexVBO);
			gpuprogram->unBindBindingTextures();
		}
		else
		{
			glPushMatrix();
			glLoadMatrixf(&modelM[0][0]);
			Texture* t = material.getDiffuseTexture();
			if(t != NULL)
			{
				t->use();
			}

			vbosubmesh->p_texcoordVBO->use();
			glTexCoordPointer(2, GL_FLOAT, 0, 0);
			vbosubmesh->p_normalVBO->use();
			glNormalPointer(GL_FLOAT, 0, 0);
			vbosubmesh->p_vertexVBO->use();
			glVertexPointer(3, GL_FLOAT, 0, 0);

			printOpenGLError();

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);

			printOpenGLError();

			vbosubmesh->p_indexVBO->use();
			glDrawElements( GL_TRIANGLES, vbosubmesh->p_indexVBO->getSize(), GL_UNSIGNED_INT, 0 );
			vbosubmesh->p_indexVBO->unUse();

			printOpenGLError();

			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);

			vbosubmesh->p_texcoordVBO->unUse();
			vbosubmesh->p_normalVBO->unUse();
			vbosubmesh->p_vertexVBO->unUse();

			printOpenGLError();
			if(t != NULL)
			{
				t->unUse();
			}
			glPushMatrix();
		}




	}


	AvancedGPUBasedVBOMeshRenderer::AvancedGPUBasedVBOMeshRenderer(const std::string& name) : GPUBasedVBOMeshRenderer(name)
	{}


	void AvancedGPUBasedVBOMeshRenderer::drawRenderSubMesh(RenderSubMesh* submesh)
	{
		VBORenderSubMesh* vbosubmesh = dynamic_cast<VBORenderSubMesh*>(submesh);
		if(NULL == vbosubmesh) return;
		glEnable( GL_TEXTURE_2D );

		Material& material = submesh->getMaterial();
		applyMaterial(&material);
		GLSLGpuProgram* gpuprogram = (GLSLGpuProgram*)material.getGpuProgram();

		if(gpuprogram != NULL)
		{
			gpuprogram->setUniformVariable("In_WorldMatrix",  this->getEntity()->getTransformation()->getGLModelMatrix());
			std::map<std::string, Texture*>& idxs = material.getTextures();
			std::map<std::string, Texture*>::iterator itor;

			for(itor = idxs.begin(); itor != idxs.end(); ++itor)
			{
				std::string bName = itor->first;
				Texture* t = itor->second;
				gpuprogram->bindTexture(bName, t);
			}

			gpuprogram->drawIndexVBO(GL_TRIANGLES, vbosubmesh->p_vertexVBO, vbosubmesh->p_normalVBO, vbosubmesh->p_texcoordVBO, vbosubmesh->p_indexVBO);
			printOpenGLError();
			gpuprogram->unBindBindingTextures();
		}else
		{
			std::cout<< "AvancedVBOMeshRenderer gpuprogram is not available : "<< gpuprogram->getName()<<std::endl;
		}


	}

}
