/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLRenderMeshRenderer.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "glhead.h"
#include "geometry/Entity.h"
#include "geometry/ObjectRenderer.h"
#include "geometry/Skeleton.h"
#include "geometry/ModelTransform.h"
#include "glfunctions.h"

namespace Etoile
{
	class GLSkeletonRenderer : public ObjectRenderer
	{
	protected:
		Skeleton* p_skeleton;
		float scale;
	public:
		GLSkeletonRenderer(const std::string& name = "") : ObjectRenderer(name), p_skeleton(NULL)
		{
		}

		void setSkeleton(Skeleton* skeleton)
		{
			p_skeleton = skeleton;
		}

		virtual void draw()
		{
			if(p_skeleton == NULL) return;
			//Matrix4f modelM;
			ModelTransform* t = this->getEntity()->getTransformation();
			scale = t->getScale().length();
			useTransform(t);
			drawSkeleton(p_skeleton);
			unUseTransform(t);
		}

		void useTransform(ModelTransform* t)
		{
			if(t)
			{
				Matrix4f modelM = t->getGLModelMatrix();
				glPushMatrix();
				glMultMatrixf(&modelM[0][0]);
			}
		}

		void unUseTransform(ModelTransform* t)
		{
			if(t)
			{
				glPopMatrix();
			}
		}


		void drawSkeleton(Skeleton* sk)
		{
			// Save OpenGL state
			glPushAttrib(GL_ALL_ATTRIB_BITS);

			GLboolean colorMaterial;
			glGetBooleanv(GL_COLOR_MATERIAL, &colorMaterial);
			glDisable(GL_COLOR_MATERIAL);

			// Set neutral GL state
			glDisable(GL_TEXTURE_1D);
			glDisable(GL_TEXTURE_2D);
			//glDisable(GL_TEXTURE_3D);

			glDisable(GL_TEXTURE_GEN_Q);
			glDisable(GL_TEXTURE_GEN_R);
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);

			//glDisable(GL_DEPTH_TEST);

			for(unsigned int i = 0; i < sk->m_joints.size(); ++i)
			{
				Joint* joint = sk->m_joints[i];
				int index = joint->m_index;
				int parent = joint->m_index_parent;
				if(parent < 0)
				{
					Vec3f& pos = sk->m_globalPositions[index];
					float color[4];
					color[0] = 0.8f;  color[1] = 0.8f;  color[2] = 0.1f;  color[3] = 0.4f;
					glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
					drawSphere_convenient(pos.x(), pos.y(), pos.z(), 0.15/scale, 10, 10);
				}
				else
				{
					Vec3f& posParent = sk->m_globalPositions[parent];
					Vec3f& pos = sk->m_globalPositions[index];
					Vec3f offset = posParent - pos;
					float len = offset.length();
					offset.normalize();
					offset /= 100.0;
					{
						float color[4];
						color[0] = 0.8f;  color[1] = 0.8f;  color[2] = 0.0f;  color[3] = 1.0f;
						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
						
						drawCylinder_convenient(pos.x() + offset.x(), pos.y()+ offset.y(), pos.z()+ offset.z(), posParent.x() - offset.x(), posParent.y() - offset.y(), posParent.z() - offset.z(), 0.3*len / scale, 10);
					}
					{
						float color[4];
						color[0] = 0.8f;  color[1] = 0.8f;  color[2] = 0.0f;  color[3] = 0.4f;
						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
						drawSphere_convenient(pos.x(), pos.y(), pos.z(), 0.3*len/scale, 10, 10);
					}
				}
			}

			if (colorMaterial)
				glEnable(GL_COLOR_MATERIAL);
			glPopAttrib();
		}

		void drawTexcoord()
		{

		}

	};

}
