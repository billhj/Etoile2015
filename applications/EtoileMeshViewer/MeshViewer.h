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

#include "geometry/RenderMesh.h"
#include "geometry/RenderManager.h"
#include "renderer/OpenGL/GLTexture2D.h"
#include "renderer/OpenGL/ImmediateMeshRenderer.h"
#include "renderer/OpenGL/LightController.h"
#include "math/Vectors.h"
#include "meshloader/OBJMeshLoader.h"
#include "geometry/Primitive.h"
#include "geometry/Camera.h"

using namespace Etoile;
class MeshViewer : public QGLViewer
{
	Q_OBJECT

	RenderManager* manager;
	Camera* p_camera;
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

		QString str = QDir::currentPath();
		
		Triangle* triangle = new Triangle("triangle");
		ImmediateMeshRenderer* renderer = new ImmediateMeshRenderer("triangle");
		Entity* entity = new Entity();
		entity->setComponent(ComponentType::RENDER_COMPONENT, renderer);
		renderer->setRenderMesh(triangle);
		RenderManager::getInstance()->addIntoObjectRendererList(renderer);
		p_camera = new Camera(Vec3f(0,0,-1), Vec3f(0,1,0), Vec3f(0,0,2));
		//p_camera->getTransform()->setPosition(Vec3f(0,0,2));
		
	}

	void readMeshFile()
	{
		QString name = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Mesh (*.obj)"));
		OBJMeshLoader objloader;
		std::cout<< "loadMesh" << std::endl;
		QTime qtime;
		qtime.start();

		RenderMesh* _pMesh = new RenderMesh("mesh");
		objloader.loadFromFile(name.toStdString(), _pMesh);
		std::map<std::string, std::string> txtPath = objloader.getTexturePathMap();
		std::vector<Material*>& mats = objloader.getMaterials();
		for(unsigned int i = 0; i < mats.size(); ++i){
			//_pRenderman->getMaterialManager()->addMaterial(new Material(mats[i]));
		}
		//loadTextures(txtPath);
		std::cout<<"loading time: "<<qtime.elapsed()<<" msc"<<std::endl;

		std::vector<RenderSubMesh*> submeshes = _pMesh->getRenderSubMeshList();
		for(unsigned int i = 0; i < submeshes.size(); ++i)
		{
			RenderSubMesh* sub = submeshes[i];
		}

		//setGPUProgramRenderer();
	}

	void draw()
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		/*p_camera->computeModelViewMatrix();
		float* modelv = p_camera->getGLModelViewMatrix();
		glLoadMatrixf(modelv);*/
		RenderManager::getInstance()->renderOneFrame();
	}

	//void loadTextures(std::map<std::string, std::string>& txtPath)
	//{
	//	std::map<std::string, std::string>::iterator itor;
	//	for(itor = txtPath.begin(); itor != txtPath.end(); ++itor)
	//	{
	//		std::string textureName = itor->first;
	//		std::string texturePath = itor->second;

	//		Image image;
	//		bool b_image = ReadImage::loadImageFromFile(texturePath, image);

	//		if(b_image != true)
	//		{
	//			std::cout<<"can not load texture : "<<textureName<<std::endl;
	//			//assert(!qimage.isNull());
	//		}
	//		else
	//		{
	//			GLTexture2D* t = new GLTexture2D(textureName);
	//			t->create(image.getWidth(), image.getHeight(),1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT ,(float*)&image.getData()[0], false);


	//		}
	//		float emptyMap[16] = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1};
	//		float checkboard[64] = {0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, 
	//			1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,0,  
	//			0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, 
	//			1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,0};
	//		GLTexture2D* t = new GLTexture2D("emptyMap");
	//		t->create(2, 2, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , &emptyMap[0], false);
	//		GLTexture2D* t2 = new GLTexture2D("checkBoardMap");
	//		t2->create(4, 4, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , &checkboard[0], false);

	//	}
	//}

	public slots:
		void setBasicRenderer()
		{
			/*_pRenderman->clear();
			std::vector<Resource*> meshes = _pRenderman->getMeshManager()->getAll();
			GLMeshRenderPass* rp = new GLMeshRenderPass("empty");
			for(unsigned int i = 0 ; i < meshes.size(); ++i)
			{
				Mesh* _pMesh = (Mesh*)meshes[i];
				DisplayListMeshRenderer* _organizer = new DisplayListMeshRenderer("");
				_organizer->setMesh(_pMesh);
				rp->addRenderUnit(_organizer);
			}
			_pRenderman->addRenderPass(rp);*/
		}

};