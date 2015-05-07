/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file PickingRenderer.cpp
* @brief 
* @date 1/2/2011
*/

#include "PickingRenderer.h"
#include "GLSLGpuProgram.h"

namespace Etoile
{
	PickingRenderer::PickingRenderer(const std::string& name) : GLMeshRenderer(name)
	{
	}

	PickingRenderer::~PickingRenderer()
	{

		for(unsigned int i = 0; i < _vboUnitList.size(); ++i)
		{
			delete _vboUnitList[i];
		}
		_vboUnitList.clear();
		_vboUnitIndex.clear();
	}

	void PickingRenderer::drawRenderMesh()
	{
		if(p_mesh == NULL) return;
		Matrix4f modelM;
		ModelTransform* t = this->getEntity()->getTransformation();
		useTransform(t);
		const std::vector<RenderSubMesh*>& submeshlist = p_mesh->getRenderSubMeshList();

		for(unsigned int i = 0; i < submeshlist.size(); ++i)
		{
			RenderSubMesh* submesh = submeshlist[i];
			drawRenderSubMesh(submesh, i);
		}
			
		drawAABB();
		unUseTransform(t);
	}
	void PickingRenderer::drawRenderSubMesh(RenderSubMesh* submesh, int idx)
	{
		glEnable( GL_TEXTURE_2D );

		Material* material = submesh->getMaterial();
		if(material != NULL){
			applyMaterial(material);

			/*Matrix4f modelM = submesh->getGLModelMatrix() * p_mesh->getGLModelMatrix();
			glPushMatrix();
			glLoadMatrixf(&modelM[0][0]);*/

			Texture* t = material->getDiffuseTexture();
			if(t != NULL)
			{
				t->use();
			}

			RenderSubMeshVBOUnit* info = _vboUnitList[idx];

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
			//glPopMatrix();
		}
		else
		{
			assert(0 && "PickingRenderer: no material");
		}

	}


	void PickingRenderer::setRenderMesh(RenderMesh* mesh)
	{
		_vboUnitList.clear();
		p_mesh = mesh;
		if(p_mesh != NULL)
			createVBO(p_mesh);

	}

	void PickingRenderer::createVBO(RenderMesh* mesh, GLenum usage)
	{
		const std::vector<RenderSubMesh*>& submeshlist = mesh->getRenderSubMeshList();

		for(unsigned int i = 0; i < submeshlist.size(); ++i)
		{
			RenderSubMesh* submesh = submeshlist[i];

			RenderSubMeshVBOUnit* info = new RenderSubMeshVBOUnit();
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

	/*void PickingRenderer::updateVBO(RenderMesh* mesh)
	{
		const std::vector<RenderSubMesh*>& submeshlist = mesh->getRenderSubMeshList();

		for(unsigned int i = 0; i < submeshlist.size(); ++i)
		{
			RenderSubMesh* submesh = submeshlist[i];
			updateVBO(submesh, i);
		}
	}

	void PickingRenderer::updateVBO(RenderSubMesh* submesh, int i)
	{
		if(submesh->getSkin()._updated)
		{
			RenderSubMeshVBOUnit* info = _vboUnitList[i];
			int size = submesh->getSkin()._vdata.size() * 3;
			info->m_vertexVBO._pVBO->write(0, size, &submesh->getSkin()._vdata[0][0]);
			info->m_normalVBO._pVBO->write(0, size, &submesh->getSkin()._ndata[0][0]);
			submesh->getSkin()._updated = false;
		}
	}*/

	int PickingRenderer::getVBOUnitIndexByName(const std::string& name)
	{
		std::map<std::string, int>::iterator itor = _vboUnitIndex.find(name);
		if(itor != _vboUnitIndex.end())
		{
			return itor->second;
		}
		return -1;
	}

	
	void PickingRenderer::processPickUbyte()
	{
		GLint viewport[4];
		GLubyte pixel[3];
		glGetIntegerv(GL_VIEWPORT,viewport);
		glReadPixels(m_cursorX,viewport[3]-m_cursorY,1,1,GL_RGB,GL_UNSIGNED_BYTE,(void *)pixel);
		printf("%d %d %d\n",pixel[0],pixel[1],pixel[2]);
	}

	void PickingRenderer::processPickInt()
	{
		GLint viewport[4];
		GLint pixel[3];
		glGetIntegerv(GL_VIEWPORT,viewport);
		glReadPixels(m_cursorX,viewport[3]-m_cursorY,1,1,GL_RGB,GL_INT,(void *)pixel);
		printf("%d %d %d\n",pixel[0],pixel[1],pixel[2]);
	}
	void PickingRenderer::processPickFloat()
	{
		GLint viewport[4];
		GLfloat pixel[3];
		glGetIntegerv(GL_VIEWPORT,viewport);
		glReadPixels(m_cursorX,viewport[3]-m_cursorY,1,1,GL_RGB,GL_FLOAT,(void *)pixel);
		printf("%d %d %d\n",pixel[0],pixel[1],pixel[2]);
	}
}
