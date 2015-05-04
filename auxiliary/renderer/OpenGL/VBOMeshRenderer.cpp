/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file VBOMeshRenderer.cpp
* @brief 
* @date 1/2/2011
*/

#include "VBOMeshRenderer.h"
#include "GLSLGpuProgram.h"

namespace Etoile
{
	VBOMeshRenderer::VBOMeshRenderer(const std::string& name) : GLMeshRenderer(name)
	{
	}

	VBOMeshRenderer::~VBOMeshRenderer()
	{

		for(unsigned int i = 0; i < _vboUnitList.size(); ++i)
		{
			delete _vboUnitList[i];
		}
		_vboUnitList.clear();
		_vboUnitIndex.clear();
	}

	void VBOMeshRenderer::draw()
	{

		drawMesh();
	}


	void VBOMeshRenderer::drawMesh()
	{
		if(p_mesh == NULL) return;
		
		const std::vector<SubMesh*>& submeshlist = p_mesh->getSubMeshList();
		for(unsigned int i = 0; i < submeshlist.size(); ++i)
		{
			SubMesh* submesh = submeshlist[i];
			drawSubMesh(submesh, i);
		}
		drawAABB();
	}

	void VBOMeshRenderer::drawSubMesh(SubMesh* submesh, int idx)
	{
		glEnable( GL_TEXTURE_2D );

		Material* material = submesh->getMaterial();
		if(material != NULL){
			applyMaterial(material);

			Matrix4f modelM = submesh->getGLModelMatrix() * p_mesh->getGLModelMatrix();
			glPushMatrix();
			glLoadMatrixf(&modelM[0][0]);

			Texture* t = material->getDiffuseTexture();
			if(t != NULL)
			{
				t->use();
			}

			SubMeshVBOUnit* info = _vboUnitList[idx];

			info->m_texCoordVBO._pVBO->use();
			glTexCoordPointer(info->m_texCoordVBO._numberComponents, GL_FLOAT, 0, 0);
			info->m_normalVBO._pVBO->use();
			glNormalPointer(GL_FLOAT, 0, 0);
			info->m_vertexVBO._pVBO->use();
			glVertexPointer(info->m_vertexVBO._numberComponents, GL_FLOAT, 0, 0);

			printOpenGLError();

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);

			printOpenGLError();

			info->p_indexVBO->use();
			glDrawElements( GL_TRIANGLES, info->p_indexVBO->getSize(), GL_UNSIGNED_INT, 0 );
			info->p_indexVBO->unUse();

			printOpenGLError();

			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);


			info->m_texCoordVBO._pVBO->unUse();
			info->m_normalVBO._pVBO->unUse();
			info->m_vertexVBO._pVBO->unUse();

			printOpenGLError();

			if(t != NULL)
			{
				t->unUse();
			}
			glPopMatrix();
		}
		else
		{
			assert(0 && "VBOMeshRenderer: no material");
		}

	}


	void VBOMeshRenderer::setMesh(Mesh* mesh)
	{
		_vboUnitList.clear();
		p_mesh = mesh;
		if(p_mesh != NULL)
			createVBO(p_mesh);

	}

	void VBOMeshRenderer::createVBO(Mesh* mesh, GLenum usage)
	{
		const std::vector<SubMesh*>& submeshlist = mesh->getSubMeshList();

		for(unsigned int i = 0; i < submeshlist.size(); ++i)
		{
			SubMesh* submesh = submeshlist[i];

			SubMeshVBOUnit* info = new SubMeshVBOUnit();
			size_t sizeComponent = submesh->getOriginalVertices().size();
			size_t sizeTextureCord = submesh->getOriginalTextureCoords().size();
	
			VBO* normalVBO = new VBO(sizeComponent * 3, &(submesh->getOriginalNormals()[0][0]), usage);
			info->m_normalVBO._pVBO = normalVBO;
			info->m_normalVBO._attributeName = "In_Normal";
			info->m_normalVBO._numberComponents = 3;
			info->m_normalVBO._primitive = GL_TRIANGLES;

			VBO* texCoordVBO = new VBO(sizeTextureCord * 2, &(submesh->getOriginalTextureCoords()[0][0]), usage);
			info->m_texCoordVBO._pVBO = texCoordVBO;
			info->m_texCoordVBO._attributeName = "In_TextureCoord";
			info->m_texCoordVBO._numberComponents = 2;
			info->m_texCoordVBO._primitive = GL_TRIANGLES;

			VBO* vertexVBO = new VBO(sizeComponent * 3, &(submesh->getOriginalVertices()[0][0]), usage);
			info->m_vertexVBO._pVBO = vertexVBO;
			info->m_vertexVBO._attributeName = "In_Vertex";
			info->m_vertexVBO._numberComponents = 3;
			info->m_vertexVBO._primitive = GL_TRIANGLES;


			IndexVBO* p_indexVBO = new IndexVBO(submesh->getOriginalVertexIndexForFaces().size(), &(submesh->getOriginalVertexIndexForFaces()[0]), usage);
			info->p_indexVBO = p_indexVBO;
			_vboUnitList.push_back(info);

		}
	}

	/*void VBOMeshRenderer::updateVBO(Mesh* mesh)
	{
		const std::vector<SubMesh*>& submeshlist = mesh->getSubMeshList();

		for(unsigned int i = 0; i < submeshlist.size(); ++i)
		{
			SubMesh* submesh = submeshlist[i];
			updateVBO(submesh, i);
		}
	}

	void VBOMeshRenderer::updateVBO(SubMesh* submesh, int i)
	{
		if(submesh->getSkin()._updated)
		{
			SubMeshVBOUnit* info = _vboUnitList[i];
			int size = submesh->getSkin()._vdata.size() * 3;
			info->m_vertexVBO._pVBO->write(0, size, &submesh->getSkin()._vdata[0][0]);
			info->m_normalVBO._pVBO->write(0, size, &submesh->getSkin()._ndata[0][0]);
			submesh->getSkin()._updated = false;
		}
	}*/

	int VBOMeshRenderer::getVBOUnitIndexByName(const std::string& name)
	{
		std::map<std::string, int>::iterator itor = _vboUnitIndex.find(name);
		if(itor != _vboUnitIndex.end())
		{
			return itor->second;
		}
		return -1;
	}

	GPUBasedVBOMeshRenderer::GPUBasedVBOMeshRenderer(const std::string& name) : VBOMeshRenderer(name)
	{}


	void GPUBasedVBOMeshRenderer::drawSubMesh(SubMesh* submesh, int idx)
	{

		glEnable( GL_TEXTURE_2D );

		Material* material = submesh->getMaterial();
		if(material != NULL)
		{
			applyMaterial(material);
			GLSLGpuProgram* gpuprogram = (GLSLGpuProgram*)material->getGpuProgram();

			Matrix4f modelM = submesh->getGLModelMatrix() * p_mesh->getGLModelMatrix();
			if(gpuprogram != NULL)
			{
				gpuprogram->setUniformVariable("In_WorldMatrix",  modelM);
				std::map<std::string, Texture*>& idxs = material->getTextures();
				std::map<std::string, Texture*>::iterator itor;

				for(itor = idxs.begin(); itor != idxs.end(); ++itor)
				{
					std::string bName = itor->first;
					Texture* t = itor->second;
					gpuprogram->bindTexture(bName, t);
				}
				SubMeshVBOUnit* info = _vboUnitList[idx];
				gpuprogram->drawIndexVBO(GL_TRIANGLES, info->m_vertexVBO, info->m_normalVBO, info->m_texCoordVBO, info->p_indexVBO);
				gpuprogram->unBindBindingTextures();
			}
			else
			{
				glPushMatrix();
				glLoadMatrixf(&modelM[0][0]);
				Texture* t = material->getDiffuseTexture();
				if(t != NULL)
				{
					t->use();
				}
				SubMeshVBOUnit* info = _vboUnitList[idx];

				info->m_texCoordVBO._pVBO->use();
				glTexCoordPointer(info->m_texCoordVBO._numberComponents, GL_FLOAT, 0, 0);
				info->m_normalVBO._pVBO->use();
				glNormalPointer(GL_FLOAT, 0, 0);
				info->m_vertexVBO._pVBO->use();
				glVertexPointer(info->m_vertexVBO._numberComponents, GL_FLOAT, 0, 0);

				printOpenGLError();

				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glEnableClientState(GL_NORMAL_ARRAY);
				glEnableClientState(GL_VERTEX_ARRAY);

				printOpenGLError();

				info->p_indexVBO->use();
				glDrawElements( GL_TRIANGLES, info->p_indexVBO->getSize(), GL_UNSIGNED_INT, 0 );
				info->p_indexVBO->unUse();

				printOpenGLError();

				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_NORMAL_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);

				info->m_texCoordVBO._pVBO->unUse();
				info->m_normalVBO._pVBO->unUse();
				info->m_vertexVBO._pVBO->unUse();

				printOpenGLError();
				if(t != NULL)
				{
					t->unUse();
				}
				glPushMatrix();
			}

		}
		else
		{
			assert(0 && "GPUBasedVBOMeshRenderer: no material");
		}

	}


	AvancedGPUBasedVBOMeshRenderer::AvancedGPUBasedVBOMeshRenderer(const std::string& name) : GPUBasedVBOMeshRenderer(name)
	{}


	void AvancedGPUBasedVBOMeshRenderer::drawSubMesh(SubMesh* submesh, int idx)
	{
		glEnable( GL_TEXTURE_2D );

		Material* material = submesh->getMaterial();
		if(material != NULL)
		{
			applyMaterial(material);
			GLSLGpuProgram* gpuprogram = (GLSLGpuProgram*)material->getGpuProgram();

			if(gpuprogram != NULL)
			{
				gpuprogram->setUniformVariable("In_WorldMatrix", submesh->getGLModelMatrix() * p_mesh->getGLModelMatrix());
				std::map<std::string, Texture*>& idxs = material->getTextures();
				std::map<std::string, Texture*>::iterator itor;

				for(itor = idxs.begin(); itor != idxs.end(); ++itor)
				{
					std::string bName = itor->first;
					Texture* t = itor->second;
					gpuprogram->bindTexture(bName, t);
				}

				SubMeshVBOUnit* info = _vboUnitList[idx];
				gpuprogram->drawIndexVBO(GL_TRIANGLES, info->m_vertexVBO, info->m_normalVBO, info->m_texCoordVBO, info->p_indexVBO);
				printOpenGLError();
				gpuprogram->unBindBindingTextures();
			}else
			{
				std::cout<< "AvancedVBOMeshRenderer gpuprogram is not available : "<< gpuprogram->getName()<<std::endl;
			}
		}
		else
		{
			assert(0 && "AvancedVBOMeshRenderer: no material");
		}

	}

}
