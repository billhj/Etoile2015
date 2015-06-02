/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GeneralLoader.h
* @brief 
* @date 1/2/2011
*/

#pragma once

#include <string>
#include "geometry/RenderMesh.h"
#include "geometry/Entity.h"
#include "meshIO/OBJMeshLoader.h"
#include "renderer/OpenGL/VBOMeshRenderer.h"
#include "geometry/RenderManager.h"
#include "geometry/SceneManager.h"
#include "meshIO/OBJSimpleMeshLoader.h"

namespace Etoile
{
	class GeneralLoader
	{
	public:
		GeneralLoader(){}
		~GeneralLoader(){}
		static Entity* loadFromFile(const std::string& fileName) 
		{
			Scene* scene = SceneManager::getInstance()->getCurrentScene();
			OBJMeshLoader loader;
			RenderMesh* mesh = new RenderMesh(fileName);
			bool created = loader.loadFromFile(fileName, mesh);
			if(created){
				VBOMeshRenderer* renderer = new VBOMeshRenderer();
				renderer->setRenderMesh(mesh);

				Entity* entity = new Entity(fileName, scene);
				entity->setComponent(ComponentType::RENDER_COMPONENT, renderer);
				RenderManager::getInstance()->addIntoObjectRendererList(renderer);
				return entity;
			}else
			{
				delete mesh;
				return NULL;
			}
		}

	};

}
