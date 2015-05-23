/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLRenderMeshRenderer.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "renderer/MeshRenderer.h"
#include "glhead.h"
#include "geometry/Entity.h"

namespace Etoile
{
	class GLMeshRenderer : public MeshRenderer
	{
	protected:
		bool m_drawAABBs;
	public:
		GLMeshRenderer(const std::string& name = "") : MeshRenderer(name), m_drawAABBs(true)
		{
		}

		void setAABBVisible(bool b)
		{
			m_drawAABBs = b;
		}

		virtual void drawRenderMesh()
		{
			if(p_mesh == NULL) return;
			//Matrix4f modelM;
			ModelTransform* t = this->getEntity()->getTransformation();
			useTransform(t);
			const std::vector<RenderSubMesh*>& submeshlist = p_mesh->getRenderSubMeshList();

			for(unsigned int i = 0; i < submeshlist.size(); ++i)
			{
				RenderSubMesh* submesh = submeshlist[i];
				drawRenderSubMesh(submesh);
			}

			//drawAABB();
			unUseTransform(t);
		}

		void useTransform(ModelTransform* t)
		{
			if(t)
			{
				Matrix4f modelM = t->getGLModelMatrix();
				glPushMatrix();
				glLoadMatrixf(&modelM[0][0]);
			}
		}

		void unUseTransform(ModelTransform* t)
		{
			if(t)
			{
				glPopMatrix();
			}
		}

		virtual void drawRenderSubMesh(RenderSubMesh* submesh)
		{
			const std::vector<Vec3f>& vertices = submesh->getVertices();
			const std::vector<Vec3f>& normals = submesh->getNormals();
			const std::vector<Vec2f>& texs = submesh->getTextureCoords();
			const std::vector<int>& faceIndices = submesh->getVertexIndexForFaces();

			Material& material = submesh->getMaterial();
			Texture* t = NULL;
			applyMaterial(&material);
			t = material.getDiffuseTexture();
			if(t != NULL)
			{
				t->use();
			}


			glBegin(GL_TRIANGLES);
			for(unsigned int i = 0; i < faceIndices.size(); ++i)
			{
				glTexCoord2fv( &texs[faceIndices[i]][0] );
				glNormal3fv( &normals[faceIndices[i]][0] );
				glVertex3fv( &vertices[faceIndices[i]][0] );
			}
			glEnd();

			if(t != NULL)
			{
				t->unUse();
			}

		}


		void drawTexcoord()
		{
			drawMeshTexcoord();
		}


		void drawMeshTexcoord()
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

		void drawSubMeshTexcoord(RenderSubMesh* submesh)
		{
			const std::vector<Vec3f>& vertices = submesh->getVertices();
			const std::vector<Vec3f>& texs = submesh->getTextureCoordsColor();

			if(texs.size() < 1 || vertices.size() < 1) return;

			const std::vector<int>& faceIndices = submesh->getVertexIndexForFaces();

			glBegin(GL_TRIANGLES);
			for(unsigned int i = 0; i < faceIndices.size(); ++i)
			{
				glColor3fv( &texs[faceIndices[i]][0] );
				glVertex3fv( &vertices[faceIndices[i]][0] );
			}
			glEnd();
		}


		void drawAABB(AxisAlignedBoundingBoxf* aabb)
		{
			Vec4f minV = Vec4f(aabb->minimum().x(), aabb->minimum().y(), aabb->minimum().z(), 1);
			Vec4f maxV = Vec4f(aabb->maximum().x(), aabb->maximum().y(), aabb->maximum().z(), 1);
			Vec3f min = Vec3f(minV.x(), minV.y(), minV.z());
			Vec3f max = Vec3f(maxV.x(), maxV.y(), maxV.z());
			glLineWidth(2.5); 
			glMaterialfv(GL_FRONT, GL_DIFFUSE, &Vec4f(0, 0, 1, 1)[0]);
			glBegin(GL_LINE_STRIP);
			glVertex3f(min.x(),min.y(),min.z());
			glVertex3f(min.x(),min.y(),max.z());
			glVertex3f(max.x(),min.y(),max.z());
			glVertex3f(max.x(),min.y(),min.z());
			glVertex3f(min.x(),min.y(),min.z());
			glEnd();
			glBegin(GL_LINE_STRIP);
			glVertex3f(min.x(),max.y(),min.z());
			glVertex3f(min.x(),max.y(),max.z());
			glVertex3f(max.x(),max.y(),max.z());
			glVertex3f(max.x(),max.y(),min.z());
			glVertex3f(min.x(),max.y(),min.z());
			glEnd();
			glBegin(GL_LINES);
			glVertex3f(min.x(),min.y(),min.z());
			glVertex3f(min.x(),max.y(),min.z());

			glVertex3f(min.x(),min.y(),max.z());
			glVertex3f(min.x(),max.y(),max.z());

			glVertex3f(max.x(),min.y(),max.z());
			glVertex3f(max.x(),max.y(),max.z());

			glVertex3f(max.x(),min.y(),min.z());
			glVertex3f(max.x(),max.y(),min.z());
			glEnd();
		}

		void drawAABB()
		{
			if(p_mesh == NULL) return;
			const std::vector<RenderSubMesh*>& submeshlist = p_mesh->getRenderSubMeshList();
			if(m_drawAABBs)
			{
				ModelTransform* t = this->getEntity()->getTransformation();
				useTransform(t);
				for(unsigned int i = 0; i < submeshlist.size(); ++i)
				{
					RenderSubMesh* submesh = submeshlist[i];
					drawAABB(submesh->getAABB());
				}
				unUseTransform(t);
			}
		}

		void applyMaterial(Material* material)
		{
			if(material == NULL) return;
			Vec4f ambient = material->getAmbientFinal();
			glMaterialfv(GL_FRONT, GL_AMBIENT, &Vec4f((float)ambient[0], (float)ambient[1], (float)ambient[2], (float)ambient[3])[0]);
			Vec4f diffuse = material->getDiffuseFinal();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, &Vec4f((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], (float)diffuse[3])[0]);
			Vec4f specular = material->getSpecularFinal();
			glMaterialfv(GL_FRONT, GL_SPECULAR, &Vec4f((float)specular[0], (float)specular[1], (float)specular[2], (float)specular[3])[0]);
			glMateriali(GL_FRONT, GL_SHININESS, material->getSpecularShininess());
			float tranparencyValue = material->getDissolvedOpacity();
			//make sure need to use  applying glEnable(GL_POLYGON_STIPPLE);
			glEnable(GL_POLYGON_STIPPLE);
			glPolygonStipple(__stippleMask[int(tranparencyValue * __screenDoorMaskRange)]);
		}


	};

}
