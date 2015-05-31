/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLSimpleMeshRenderer.cpp
* @brief 
* @date 1/2/2011
*/

#include "GLSimpleMeshRenderer.h"
#include "glhead.h"
#include "geometry/TextureManager.h"
#include "geometry/Texture.h"

namespace Etoile
{
	GLSimpleMeshRenderer::GLSimpleMeshRenderer(const std::string& name) : ObjectRenderer(name), p_mesh(NULL)
	{
#ifdef USING_GLEW
		p_vertexVBO = NULL;
		p_normalVBO = NULL;
		p_texcoordVBO = NULL;
		p_indexVBO = NULL;
#endif
	}

	GLSimpleMeshRenderer::~GLSimpleMeshRenderer()
	{
#ifdef USING_GLEW
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
#endif
	}

	void GLSimpleMeshRenderer::draw()
	{
		drawSimpleMesh(p_mesh);
	}

	void useMaterial(SimpleMesh::Material& mat)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, &mat.m_ambient[0]);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, &mat.m_diffuse[0]);
		glMaterialfv(GL_FRONT, GL_SPECULAR, &mat.m_specular[0]);
		glMateriali(GL_FRONT, GL_SHININESS, mat.m_shininess);
		//make sure need to use  applying glEnable(GL_POLYGON_STIPPLE);
		glEnable(GL_POLYGON_STIPPLE);
		glPolygonStipple(__stippleMask[int(mat.m_transparency * __screenDoorMaskRange)]);
		int index = mat.m_indicesInRessouce[TextureMaterial::DIFFUSE_MAP];
		if(index >= 0)
		{
			TextureManager::getInstance()->getTextureByIndex(index)->use();
		}
	}

	void GLSimpleMeshRenderer::setSimpleMesh(SimpleMesh* mesh)
	{
		p_mesh = mesh;
#ifdef USING_GLEW
		GLenum usage = GL_STATIC_DRAW_ARB;
		p_normalVBO = new VBO(mesh->m_normals.size() * 3, &(mesh->m_normals[0][0]), usage);
		p_texcoordVBO = new VBO(mesh->m_texcoords.size() * 3, &(mesh->m_texcoords[0][0]), usage);
		p_vertexVBO = new VBO(mesh->m_positions.size() * 3, &(mesh->m_positions[0][0]), usage);
		p_indexVBO = new IndexVBO(mesh->m_vertexIndices.size(), &(mesh->m_vertexIndices[0]), usage);
#endif
	}

#ifdef USING_GLEW
	void GLSimpleMeshRenderer::setSimpleMesh(SimpleMesh* mesh, GLenum usage)
	{
		p_mesh = mesh;
		p_normalVBO = new VBO(mesh->m_normals.size() * 3, &(mesh->m_normals[0][0]), usage);
		p_texcoordVBO = new VBO(mesh->m_texcoords.size() * 3, &(mesh->m_texcoords[0][0]), usage);
		p_vertexVBO = new VBO(mesh->m_positions.size() * 3, &(mesh->m_positions[0][0]), usage);
		p_indexVBO = new IndexVBO(mesh->m_vertexIndices.size(), &(mesh->m_vertexIndices[0]), usage);
	}

	void GLSimpleMeshRenderer::drawSimpleMesh(SimpleMesh* mesh)
	{
		for(unsigned int i = 0; i < mesh->m_groups.size(); ++i)
		{
			SimpleMesh::Group& group = mesh->m_groups[i];
			SimpleMesh::Material& mat = mesh->m_materials[group.m_materialIndex];
			useMaterial(mat);

			p_texcoordVBO->use();
			glTexCoordPointer(3, GL_FLOAT, 0, 0);
			p_normalVBO->use();
			glNormalPointer(GL_FLOAT, 0, 0);
			p_vertexVBO->use();
			glVertexPointer(3, GL_FLOAT, 0, 0);

			printOpenGLError();

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);

			printOpenGLError();

			p_indexVBO->use();
			glDrawElements( GL_TRIANGLES, group.m_count_vertexIndices, GL_UNSIGNED_INT, (GLvoid*)(group.m_offset_vertexIndices));
			p_indexVBO->unUse();

			printOpenGLError();

			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);


			p_texcoordVBO->unUse();
			p_normalVBO->unUse();
			p_vertexVBO->unUse();

			printOpenGLError();
		}
	}
#else
	void GLSimpleMeshRenderer::drawSimpleMesh(SimpleMesh* mesh)
	{
		for(unsigned int i = 0; i < mesh->m_groups.size(); ++i)
		{
			SimpleMesh::Group& group = mesh->m_groups[i];
			SimpleMesh::Material& mat = mesh->m_materials[group.m_materialIndex];
			useMaterial(mat);
			glBegin(GL_TRIANGLES);
			for(unsigned int j = 0; j < group.m_faceIndices.size(); ++j)
			{
				SimpleMesh::Face& face = mesh->m_faces[j];
				for(unsigned int k = 0; k < face.m_verticesInfo.size(); ++k)
				{
					SimpleMesh::Vertex& v = face.m_verticesInfo[k];
					glTexCoord3fv(&mesh->m_texcoords[v.m_texcoordIndex][0]);
					glNormal3fv(&mesh->m_normals[v.m_normalIndex][0]);
					glVertex3fv(&mesh->m_positions[v.m_posIndex][0]);
				}
			}
			glEnd();
		}
	}
#endif

	
}
