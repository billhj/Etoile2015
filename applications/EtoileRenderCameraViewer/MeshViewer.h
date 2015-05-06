/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MeshViewer.h
* @brief 
* @date 1/2/2011
*/

#include "renderer/OpenGL/glhead.h"
#include "renderer/OpenGL/glInfo.h"
#include "QGLRenderWidget.h"

#include "geometry/RenderMesh.h"
#include "geometry/RenderManager.h"
#include "renderer/OpenGL/GLTexture2D.h"
#include "renderer/OpenGL/VBOMeshRenderer.h"
#include "renderer/OpenGL/LightController.h"
#include "math/Vectors.h"
#include "meshloader/OBJMeshLoader.h"
#include "geometry/Primitive.h"
#include "geometry/Camera.h"

using namespace Etoile;
class MeshViewer : public QGLRenderWidget
{
	Q_OBJECT

	RenderManager* manager;
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
		
		Triangle* triangle = new Triangle("triangle");
		VBOMeshRenderer* renderer = new VBOMeshRenderer("triangle");
		Entity* entity = new Entity();
		entity->setComponent(ComponentType::RENDER_COMPONENT, renderer);
		renderer->setRenderMesh(triangle);
		manager = new RenderManager("render");
		manager->addIntoObjectRendererList(renderer);
		//p_camera->getTransform()->setPosition(Vec3f(0,0,2));
		
	}

	

	void draw()
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		manager->renderOneFrame();
		Vec3f cameraPos = p_camera->getTransform()->getPosition();
		//std::cout<<cameraPos<<std::endl;
	}

	

};