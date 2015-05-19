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
MeshViewer::MeshViewer(QWidget *parent) : QGLRenderWidget(parent), m_pickOn(false)
{
	m_pixel = new GLfloat[3];
}

MeshViewer::~MeshViewer()
{
	delete m_pixel;
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
	//this->setFullScreen();
	
}

void MeshViewer::drawOnTexture()
{
	if(m_tool == PENCIL && m_mode == TEXTURE_MODE && m_pickOn == true)
	{
		texCoordPicking();
		m_x, m_y;
		m_pixel;
		std::cout<<"drawOnTexture: screen "<<m_x <<" "<<m_y<<" texCoord: "<< m_pixel[0]<<"  "<<m_pixel[1]<<std::endl;
		m_pickOn = false;
		//TODO drawOnTexture
	}
}


void MeshViewer::draw()
{


	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	drawOnTexture();

	/*this is use for no lighting gl_color ()
	glDisable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColor4f(0,0,1,1);*/
	RenderManager::getInstance()->renderOneFrame();
	
	colorPicking();
	
}


void MeshViewer::texCoordPicking()
{
	GLboolean lighting, colorMaterial;
	glGetBooleanv(GL_LIGHTING, &lighting);
	glGetBooleanv(GL_COLOR_MATERIAL, &colorMaterial);

	glDisable(GL_LIGHTING);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	RenderManager::getInstance()->renderTexcoordPicking();
	//GLfloat pixel[3];
	Picker::processPickFloat(m_x, m_y, m_pixel);
	//std::cout<<pixel[0]<<"  "<<pixel[1]<<"  "<<pixel[3]<<std::endl;
	if (colorMaterial)
		glEnable(GL_COLOR_MATERIAL);
	if (lighting)
		glEnable(GL_LIGHTING);
}

void MeshViewer::colorPicking()
{
	if(m_tool == PIPETTE && m_pickOn == true)
	{
		Picker::processPickFloat(m_x, m_y, m_pixel);
		m_pickOn = false;
		std::cout<<"colorPicking: "<<m_x <<" "<<m_y<<" color: "<< m_pixel[0]<<"  "<<m_pixel[1]<<"  "<<m_pixel[2]<<std::endl;
		emit colorPicked();
		//TODO Show on colorpanel
	}
}

void MeshViewer::mouseDoubleClickEvent(QMouseEvent* const event)
{
	m_x = event->x();
	m_y = event->y();
	m_pickOn = true;
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
	}
	else
	{	
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

void MeshViewer::setViewerMode(QString mode)
{
	if(mode.compare("VIEW") == 0)
	{
		m_mode = VIEW_MODE;
	}
	else if(mode.compare("TEXTURE") == 0)
	{
		m_mode = TEXTURE_MODE;
	}
	else if(mode.compare("GEOMETRY") == 0)
	{
		m_mode = GEOMETRY_MODE;
	}
}

void MeshViewer::setTool(QAbstractButton* button)
{
	if(button->objectName().compare("PENCIL", Qt::CaseInsensitive) == 0)
	{
		m_tool = PENCIL;
	}
	else if(button->objectName().compare("MOVE", Qt::CaseInsensitive) == 0)
	{
		m_tool = MOVE;
	}
	else if(button->objectName().compare("PIPETTE", Qt::CaseInsensitive) == 0)
	{
		m_tool = PIPETTE;
	}
}