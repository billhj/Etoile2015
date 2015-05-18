/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MeshViewer.cpp
* @brief 
* @date 1/2/2011
*/

#include "MeshViewer.h"
#include "geometry/RenderMesh.h"
#include "geometry/RenderManager.h"
#include "renderer/OpenGL/GLTexture2D.h"
#include "renderer/OpenGL/ImmediateMeshRenderer.h"
#include "renderer/OpenGL/LightController.h"
#include "math/Vectors.h"
#include "meshIO/OBJMeshLoader.h"
#include "geometry/Primitive.h"
#include "geometry/Camera.h"
#include "renderer/OpenGL/Picker.h"
#include <QMouseEvent>

using namespace Etoile;

MeshViewer::MeshViewer(QWidget *parent) : QGLRenderWidget(parent), m_picked(NO_MODE)
{

}

MeshViewer::~MeshViewer()
{
}

void MeshViewer::init()
{
	glInfo* info = glInfo::getInstance();
	info->initAllOpenGL();


	setMouseTracking(true);
	setFocusPolicy(Qt::StrongFocus);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT,GL_FILL);

	glClearColor(0.2, 0.2, 0.2, 0.99);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_MULTISAMPLE);

	Triangle* triangle = new Triangle();
		triangle->init();
		RenderMesh* mesh = new RenderMesh("triangle");
		RenderSubMesh* submesh = new RenderSubMesh("triangle");
		mesh->addRenderSubMesh(submesh);
		submesh->m_vdata = triangle->m_vdata;
		submesh->m_tdata = triangle->m_tdata;
		submesh->m_ndata = triangle->m_ndata;
		submesh->m_tcdata = triangle->m_tcdata;
		submesh->m_vertices_index_face = triangle->m_vertices_index_face;
	ImmediateMeshRenderer* renderer = new ImmediateMeshRenderer("triangle");
	Entity* entity = new Entity();
	entity->setComponent(ComponentType::RENDER_COMPONENT, renderer);
	renderer->setRenderMesh(mesh);
	RenderManager::getInstance()->addIntoObjectRendererList(renderer);
	//p_camera->getTransform()->setPosition(Vec3f(0,0,2));

}



void MeshViewer::draw()
{



	if(m_picked == TEXCOORD)
	{
		drawTexCoordPicking();
	}

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


	/*this is use for no lighting gl_color ()
	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColor4f(0,0,1,1);*/
	RenderManager::getInstance()->renderOneFrame();
	if(m_picked == COLORCOORD)
	{
		colorPicking();
	}


	//Vec3f cameraPos = p_camera->getTransform()->getPosition();
	//std::cout<<cameraPos<<std::endl;
}


void MeshViewer::drawTexCoordPicking()
{
	GLboolean lighting, colorMaterial;
	glGetBooleanv(GL_LIGHTING, &lighting);
	glGetBooleanv(GL_COLOR_MATERIAL, &colorMaterial);

	glDisable(GL_LIGHTING);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	RenderManager::getInstance()->renderTexcoordPicking();
	GLfloat pixel[3];
	Picker::processPickFloat(m_x, m_y, pixel);
	//std::cout<<pixel[0]<<"  "<<pixel[1]<<"  "<<pixel[3]<<std::endl;
	if (colorMaterial)
		glEnable(GL_COLOR_MATERIAL);
	if (lighting)
		glEnable(GL_LIGHTING);
}

void MeshViewer::colorPicking()
{
	GLfloat pixel[3];
	Picker::processPickFloat(m_x, m_y, pixel);
}

void MeshViewer::mouseDoubleClickEvent(QMouseEvent* const event)
{
	m_x = event->x();
	m_y = event->y();
	//m_picked = true;
	QGLRenderWidget::mouseDoubleClickEvent(event);
}


void MeshViewer::mousePressEvent(QMouseEvent* const event)
{
	QGLRenderWidget::mousePressEvent(event);
}

void MeshViewer::mouseReleaseEvent(QMouseEvent* const event)
{
	QGLRenderWidget::mouseReleaseEvent(event);
}

void MeshViewer::mouseMoveEvent(QMouseEvent* const event)
{
	if(event->buttons() & Qt::MouseButton::LeftButton  && event->modifiers() == Qt::CTRL)
	{
		std::cout<<"draw"<<std::endl;
		m_mode = PAINT_TEXTURE_MODE;
	}
	else
	{	//m_mode == 0;
		QGLRenderWidget::mouseMoveEvent(event);
	}
}

void MeshViewer::wheelEvent(QWheelEvent* const event)
{
	QGLRenderWidget::wheelEvent(event);
}

void MeshViewer::keyPressEvent(QKeyEvent * const event)
{
	QGLRenderWidget::keyPressEvent(event);
}

void MeshViewer::usePipette()
{

}