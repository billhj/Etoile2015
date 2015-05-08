/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GLLightRenderer.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "geometry/ObjectRenderer.h"
#include "glhead.h"
#include "geometry/Light.h"
#include "geometry/ModelTransform.h"
#include "glDrawFunctions.h"

namespace Etoile
{
	class GLLightRenderer : public ObjectRenderer
	{
		Light* p_light;
		float m_scale;
	public:
		GLLightRenderer(const std::string& name, Light* light = NULL) : ObjectRenderer(name), p_light(light)
		{
		}

		GLLightRenderer(Light* light) : ObjectRenderer(light->getName()), p_light(light)
		{

		}

		void setLight(Light* light)
		{
			p_light = light;
		}

		virtual void draw()
		{
			static GLUquadric* quadric = gluNewQuadric();
			Matrix4f modelM = p_light->getTransform()->getGLModelMatrix();

			glColor4fv(&p_light->getDiffuseFinal()[0]);
			GLfloat cutOff = p_light->getFieldOfView();
			float sceneRadius = 2.0;
			const float length = sceneRadius / 5.0 * m_scale;

			glPushMatrix();
			glLoadMatrixf(&modelM[0][0]);
			glDrawFunctions::drawArrow(length);
			gluCylinder(quadric, 0.0, 0.7 * length * sin(cutOff * M_PI / 180.0), 0.7 * length * cos(cutOff * M_PI / 180.0), 12, 1);
			glPopMatrix();


			glPushMatrix();
			glLoadIdentity();
		}

		void drawLight(GLenum light, float scale)
		{
			static GLUquadric* quadric = gluNewQuadric();
			float sceneRadius = 2.0;
			const float length = sceneRadius / 5.0 * scale;

			GLboolean lightIsOn;
			glGetBooleanv(light, &lightIsOn);

			if (lightIsOn)
			{
				// All light values are given in eye coordinates
				glPushMatrix();
				glLoadIdentity();

				float color[4];
				glGetLightfv(light, GL_DIFFUSE, color);
				glColor4fv(color);

				float pos[4];
				glGetLightfv(light, GL_POSITION, pos);

				if (pos[3] != 0.0)
				{
					glTranslatef(pos[0]/pos[3], pos[1]/pos[3], pos[2]/pos[3]);

					GLfloat cutOff;
					glGetLightfv(light, GL_SPOT_CUTOFF, &cutOff);
					if (cutOff != 180.0)
					{
						GLfloat dir[4];
						glGetLightfv(light, GL_SPOT_DIRECTION, dir);
						Quaternionf rotation(Vec3f(0,0,1), Vec3f(dir[0], dir[1], dir[2]));
						static Matrix4f r;
						rotation.getMatrix(r);
						glMultMatrixf(r.getGLMatrix());
						glDrawFunctions::drawArrow(length);
						gluCylinder(quadric, 0.0, 0.7 * length * sin(cutOff * M_PI / 180.0), 0.7 * length * cos(cutOff * M_PI / 180.0), 12, 1);
					}
					else
						gluSphere(quadric, 0.2*length, 10, 10);
				}


				glPopMatrix();
			}
		}


	};

}
