/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MeshViewer.h
* @brief 
* @date 1/2/2011
*/

#include "renderer/OpenGL/glhead.h"
#include "renderer/OpenGL/glInfo.h"
#include "QTUI/QGLRenderWidget.h"

#include "geometry/RenderMesh.h"
#include "geometry/RenderManager.h"
#include "renderer/OpenGL/GLTexture2D.h"
#include "renderer/OpenGL/ImmediateMeshRenderer.h"
#include "renderer/OpenGL/LightController.h"
#include "math/Vectors.h"
#include "meshIO/OBJMeshLoader.h"
#include "geometry/Primitive.h"
#include "geometry/Camera.h"

using namespace Etoile;
class MeshViewer : public QGLRenderWidget
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
		
		Triangle* triangle = new Triangle();
		triangle->init();
		RenderMesh* mesh = new RenderMesh("triangle");
		RenderSubMesh* submesh = new RenderSubMesh("triangle");
		mesh->addRenderSubMesh(submesh);
		submesh->m_vdata = triangle->m_vdata;
		submesh->m_tdata = triangle->m_tdata;
		submesh->m_ndata = triangle->m_ndata;
		submesh->m_vertices_index_face = triangle->m_vertices_index_face;

		renderer = new ImmediateMeshRenderer("triangle");
		Entity* entity = new Entity();
		entity->setComponent(ComponentType::RENDER_COMPONENT, renderer);
		renderer->setRenderMesh(mesh);
		RenderManager::getInstance()->add(renderer);
		//p_camera->getTransform()->setPosition(Vec3f(0,0,2));
		
	}
	ImmediateMeshRenderer* renderer;
	

	void draw()
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		//RenderManager::getInstance()->renderOneFrame();
		RenderManager::getInstance()->renderTexcoord();
		//Vec3f cameraPos = p_camera->getTransform()->getPosition();
		//std::cout<<cameraPos<<std::endl;
	}

	

};