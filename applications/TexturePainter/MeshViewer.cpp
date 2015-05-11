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
#include "meshloader/OBJMeshLoader.h"
#include "geometry/Primitive.h"
#include "geometry/Camera.h"
#include "renderer/OpenGL/Picker.h"
#include <QMouseEvent>

using namespace Etoile;

MeshViewer::MeshViewer(QWidget *parent) : QGLRenderWidget(parent)
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
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_MULTISAMPLE);

	Triangle* triangle = new Triangle("triangle");
	ImmediateMeshRenderer* renderer = new ImmediateMeshRenderer("triangle");
	Entity* entity = new Entity();
	entity->setComponent(ComponentType::RENDER_COMPONENT, renderer);
	renderer->setRenderMesh(triangle);
	RenderManager::getInstance()->addIntoObjectRendererList(renderer);
	//p_camera->getTransform()->setPosition(Vec3f(0,0,2));

}



void MeshViewer::draw()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//RenderManager::getInstance()->renderOneFrame();
	RenderManager::getInstance()->renderTexcoordPicking();
	if(m_picked)
	{
		GLfloat pixel[3];
		Picker::processPickFloat(m_x, m_y, pixel);
		std::cout<<pixel[0]<<"  "<<pixel[1]<<"  "<<pixel[3]<<std::endl;
		m_picked = false;
	}
	//Vec3f cameraPos = p_camera->getTransform()->getPosition();
	//std::cout<<cameraPos<<std::endl;
}


void MeshViewer::mouseDoubleClickEvent(QMouseEvent* const event)
{
	m_x = event->x();
	m_y = event->y();
	m_picked = true;
	QGLRenderWidget::mouseDoubleClickEvent(event);
}
