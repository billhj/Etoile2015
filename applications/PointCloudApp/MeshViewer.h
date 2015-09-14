/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MeshViewer.h
* @brief 
* @date 1/2/2011
*/

#include "renderer/OpenGL/glhead.h"
#include "renderer/OpenGL/glInfo.h"
#include <QGLViewer/qglviewer.h>
#include <QImage>
#include <QTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include "RenderManager.h"


using namespace Etoile;
class MeshViewer : public QGLViewer
{
	Q_OBJECT

public:
	MeshViewer()
	{
		
	}

	~MeshViewer()
	{
		clear();
	}

	void clear()
	{

	}

	void init()
	{
		glInfo* info = glInfo::getInstance();
		info->initAllOpenGL();


		setMouseTracking(true);
		setFocusPolicy(Qt::StrongFocus);
		setShortcut(DISPLAY_FPS,Qt::CTRL+Qt::Key_F);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT,GL_FILL);

		glClearColor(0, 0, 0, 0.99);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_MULTISAMPLE);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
		QString str = QDir::currentPath();
		this->setSceneRadius(100);
		this->camera()->setPosition(qglviewer::Vec(0,0,50));
	}


	void draw()
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		RenderManager::getInstance()->renderOneFrame();
	}

};