/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file QTMeshLoader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <QThread>
#include "meshIO/OBJMeshLoader.h"
#include "renderer/OpenGL/ImmediateMeshRenderer.h"
#include "renderer/OpenGL/VBOMeshRenderer.h"
#include "geometry/RenderManager.h"
//#include "QTUI/QTTextureLoader.h"

namespace Etoile
{
	class QTMeshLoader : public QThread
	{
		Q_OBJECT

		QWidget* m_parent;
		std::string m_filepath;
	public:
		QTMeshLoader(QWidget* parent = NULL): m_parent(parent){}
		void setMeshFilePath(const std::string& filepath)
		{
			m_filepath = filepath;
		}
		void run() 
		{
			
			/*OBJMeshLoader loader;
			QTTextureLoader qtextureLoader;
			loader.setTextureLoader(&qtextureLoader);

			RenderMesh* mesh = new RenderMesh(m_filepath);
			loader.loadFromFile(m_filepath, mesh);

			VBOMeshRenderer* renderer = new VBOMeshRenderer();
			renderer->setRenderMesh(mesh);

			Entity* entity = new Entity(m_filepath);
			entity->setComponent(ComponentType::RENDER_COMPONENT, renderer);
			RenderManager::getInstance()->addIntoObjectRendererList(renderer);*/

			emit meshReady();
		}
signals:
		void meshReady();

	};


}
