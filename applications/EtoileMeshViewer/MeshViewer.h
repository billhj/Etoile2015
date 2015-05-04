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

#include "renderer/Mesh.h"
#include "renderer/MeshLoader/OBJMeshLoader.h"
#include "renderer/OpenGL/GLTexture2D.h"
#include "renderer/OpenGL/UniformBufferObject.h"
#include "renderer/OpenGL/ImmediateMeshRenderUnit.h"
#include "util/ImageLoader/ReadImage.h"
#include "renderer/OpenGL/GLMeshRenderPass.h"
#include "renderer/OpenGL/QuadRenderPass.h"
#include "renderer/OpenGL/GLMultiRenderTarget.h"
#include "renderer/OpenGL/LightController.h"
#include "GpuProgramInit.h"
#include "RendererInit.h"
#include "LightInit.h"
#include "Core.h"
#include "util/SamplesGenerator.h"
#include "math/Vectors.h"
#include "util/Dir.h"
using namespace Etoile;
class MeshViewer : public QGLViewer
{
	Q_OBJECT
public:
	MeshViewer(): _pRenderman(NULL)
	{
	}

	~MeshViewer()
	{
		clear();
	}

	void clear()
	{
		if(_pRenderman != NULL)
			delete _pRenderman;
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

		_pRenderman = new Renderer("my", &_core);
		_pTransform = new UniformBufferObject(4 * sizeof(Matrix4f), GL_DYNAMIC_DRAW, 0);
		QString str = QDir::currentPath();
		const std::string dir = Dir::getCurrentDirectory();
		_initG.init(_pRenderman, str.toStdString() + "/shader/gpu.xml");
		_initL.init(_pRenderman, str.toStdString() + "/shader/light.xml");
	}

	void readMeshFile()
	{
		QString name = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Mesh (*.obj)"));
		OBJMeshLoader objloader;
		std::cout<< "loadMesh" << std::endl;
		QTime qtime;
		qtime.start();

		Mesh* _pMesh = new Mesh("mesh");
		_pRenderman->getMeshManager()->addMesh(_pMesh);
		objloader.loadFromFile(name.toStdString(), _pMesh);
		std::map<std::string, std::string> txtPath = objloader.getTexturePathMap();
		std::vector<Material>& mats = objloader.getMaterials();
		for(unsigned int i = 0; i < mats.size(); ++i){
			_pRenderman->getMaterialManager()->addMaterial(new Material(mats[i]));
		}
		loadTextures(txtPath);
		std::cout<<"loading time: "<<qtime.elapsed()<<" msc"<<std::endl;

		std::vector<SubMesh*> submeshes = _pMesh->getSubMeshList();
		for(unsigned int i = 0; i < submeshes.size(); ++i)
		{
			SubMesh* sub = submeshes[i];
			std::string materialname = sub->getMaterial();
			Material* m = (Material*)_pRenderman->getMaterialManager()->getMaterialByName(materialname);
			m->setGpuProgram("drawmesh");
		}

		setBasicRenderer();
		//setGPUProgramRenderer();
	}

	void draw()
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		if(_pRenderman != NULL)
		{
			updateLight();
			updateMatrix();
			_pRenderman->renderOneFrame();
		}
	}

	void loadTextures(std::map<std::string, std::string>& txtPath)
	{
		std::map<std::string, std::string>::iterator itor;
		for(itor = txtPath.begin(); itor != txtPath.end(); ++itor)
		{
			std::string textureName = itor->first;
			std::string texturePath = itor->second;

			Image image;
			bool b_image = ReadImage::loadImageFromFile(texturePath, image);

			if(b_image != true)
			{
				std::cout<<"can not load texture : "<<textureName<<std::endl;
				//assert(!qimage.isNull());
			}
			else
			{
				GLTexture2D* t = new GLTexture2D(textureName);
				t->create(image.getWidth(), image.getHeight(),1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT ,(float*)&image.getData()[0], false);
				_pRenderman->getTextureManager()->addTexture(t);
			}

		}
		float emptyMap[16] = {1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1};
		float checkboard[64] = {0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, 
			1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,0,  
			0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, 
			1,1,1,1,0,0,0,0, 1,1,1,1,0,0,0,0};
		GLTexture2D* t = new GLTexture2D("emptyMap");
		t->create(2, 2, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , &emptyMap[0], false);
		GLTexture2D* t2 = new GLTexture2D("checkBoardMap");
		t2->create(4, 4, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , &checkboard[0], false);
		_pRenderman->getTextureManager()->addTexture(t);
		_pRenderman->getTextureManager()->addTexture(t2);

	}

	void updateLight()
	{
		std::vector<Resource*> lights = _pRenderman->getLightManager()->getAll();
		for(unsigned int i = 0; i < lights.size(); ++i)
		{
			Light* light = (Light*)lights[i];
			LightController controller;
			controller.bindLight(light);
			controller.use();
		}
	}

	void updateMatrix()
	{
		Matrix4f transform[4];
		glMatrixMode (GL_PROJECTION);
		glGetFloatv(GL_PROJECTION_MATRIX , &(transform[0][0]));
		printOpenGLError ();
		glMatrixMode(GL_MODELVIEW);
		glGetFloatv(GL_MODELVIEW_MATRIX ,&(transform[1][0]));
		printOpenGLError ();
		transform[2] = transform[0] * transform[1];
		transform[3] = transform[1];
		transform[3].inverse();
		transform[3].transpose();
		_pTransform->writeData(4 * sizeof(Matrix4f), &transform[0][0]);
		std::vector<Resource*>& _gpuprograms = _pRenderman->getGpuProgramManager()->getAll();
		for(unsigned int i = 0; i < _gpuprograms.size(); ++i)
		{
			GLSLGpuProgram* gpupragram = ( GLSLGpuProgram*)_gpuprograms[i];
			if(gpupragram != NULL)
			{
				gpupragram->setUniformBufferObject("In_Transform", _pTransform);
				gpupragram->setUniformVariable("In_ViewMatrix", transform[1]);
				gpupragram->setUniformVariable("In_ProjectionMatrix", transform[0]);
				gpupragram->setUniformVariable("In_ViewProjectionMatrix", transform[2]);
				gpupragram->setUniformVariable("In_NormalMatrix", transform[3]);
				//gpuprogamcontroller->setGpuProgramParameter("viewDirection", Vec3f (this->camera()->viewDirection()[0],this->camera()->viewDirection()[1],this->camera()->viewDirection()[2] ));
			}
		}
	}

	void generateSampling()
	{
		HammersleySamplesGenerator* sg = new HammersleySamplesGenerator("Hammersley");
		sg->setSize(16);
		sg->setPatternSize(4);
		sg->generateSamples();
		std::vector<SamplesGenerator::Sample> samples = sg->getSamples();
	
		std::vector<Resource*>& _gpuprograms = _pRenderman->getGpuProgramManager()->getAll();
		for(unsigned int i = 0; i < _gpuprograms.size(); ++i)
		{
			GLSLGpuProgram* gpupragram = ( GLSLGpuProgram*)_gpuprograms[i];
			if(gpupragram != NULL)
			{
				gpupragram->setUniformVariable("PatternSamples", VecConverter::convertFromDoubleToFloat(samples));	
				gpupragram->setUniformVariable("SamplesNumber", int(16));
				gpupragram->setUniformVariable("PatternSize", int(4));
			}
		}
	}

	public slots:
		void setBasicRenderer()
		{
			_pRenderman->clear();
			std::vector<Resource*> meshes = _pRenderman->getMeshManager()->getAll();
			GLMeshRenderPass* rp = new GLMeshRenderPass("empty");
			for(unsigned int i = 0 ; i < meshes.size(); ++i)
			{
				Mesh* _pMesh = (Mesh*)meshes[i];
				DisplayListMeshRenderUnit* _organizer = new DisplayListMeshRenderUnit("");
				_organizer->setMesh(_pMesh,_pRenderman);
				rp->addRenderUnit(_organizer);
			}
			_pRenderman->addRenderPass(rp);
		}

		void setBasicGPUProgramRenderer()
		{
			_pRenderman->clear();
			GLMeshRenderPass* pass = new GLMeshRenderPass("drawmesh");
			std::vector<Resource*> meshes = _pRenderman->getMeshManager()->getAll();
			for(unsigned int i = 0 ; i < meshes.size(); ++i)
			{
				Mesh* _pMesh = (Mesh*)meshes[i];
				pass->addMesh(_pMesh);
			}
			_pRenderman->addRenderPass(pass);
		}

		void setGPUProgramRenderer()
		{

			_pRenderman->clear();
			GLMeshRenderPass* pass = new GLMeshRenderPass("drawmesh");
			std::vector<Resource*> meshes = _pRenderman->getMeshManager()->getAll();
			for(unsigned int i = 0 ; i < meshes.size(); ++i)
			{
				Mesh* _pMesh = (Mesh*)meshes[i];
				pass->addMesh(_pMesh);
			}
			_pRenderman->addRenderPass(pass);

			GLMultiRenderTarget* target = new GLMultiRenderTarget("drawmesh", 800, 600);
			pass->setRenderTarget(target);

			GLTexture2D* t = new GLTexture2D("normalMap");
			t->create(800, 600, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , 0, false);
			target->addTextureTarget(t->getName());
			_pRenderman->getTextureManager()->addTexture(t);
			GLTexture2D* t2 = new GLTexture2D("positionMap");
			t2->create(800, 600, 1 , GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT , 0, false);
			target->addTextureTarget(t2->getName());
			_pRenderman->getTextureManager()->addTexture(t2);

			QuadRenderPass* quadpass = new QuadRenderPass("output");
			Material* mt= new Material("output"); 
			mt->setGpuProgram("output");
			mt->setTexture("output", "normalMap");
			quadpass->setMaterial(mt->getName());
			_pRenderman->getMaterialManager()->addMaterial(mt);
			_pRenderman->addRenderPass(quadpass);

		}

		void setPassFileRenderer()
		{
			QString str = QDir::currentPath();
			QString name = QFileDialog::getOpenFileName(this, tr("Open File"),str,tr("renderer (*.xml)"));
			std::cout<< "loadRenderer" << std::endl;
			if(!name.isEmpty())
			{
				_pRenderman->clear();
				_initR.init(_pRenderman, name.toStdString());
			}
		}

		void loadGpuPro()
		{
			QString str = QDir::currentPath();
			QString name = QFileDialog::getOpenFileName(this, tr("Open File"),str,tr("gpuprogram (*.xml)"));
			std::cout<< "loadGPUpro" << std::endl;
			if(!name.isEmpty())
			{
				_initG.init(_pRenderman, name.toStdString());
				generateSampling();
			}
		}

		void reloadGpuPro()
		{
			_initG.reInit(_pRenderman);
			generateSampling();
		}

		void reloadPassRenderer()
		{
			_initR.reInit(_pRenderman);
		}


		void loadLight()
		{
			QString str = QDir::currentPath();
			QString name = QFileDialog::getOpenFileName(this, tr("Open File"),str,tr("light (*.xml)"));
			std::cout<< "loadLight" << std::endl;
			if(!name.isEmpty())
			{
				_initL.init(_pRenderman, name.toStdString());
			}
		}

		void reloadLight()
		{
			_initL.reInit(_pRenderman);
		}

private:
	Renderer* _pRenderman;
	UniformBufferObject* _pTransform;
	std::map<std::string, GLuint> _textureMap;
	RendererInit _initR;
	GpuProgramInit _initG;
	LightInit _initL;
	Core _core;
};
