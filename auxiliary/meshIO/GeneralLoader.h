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
#include "renderer/OpenGL/GLSkeletonRenderer.h"
#include "meshIO/SkeletonTextFileLoader.h"
#include "util/File.h"
#include "animation/BVH.h"
#include "animation/BVHSkeletonAnimator.h"
#include "animation/SkeletonConverter.h"

namespace Etoile
{
	class GeneralLoader
	{
	public:
		GeneralLoader(){}
		~GeneralLoader(){}

		static Entity* loadFromFile(const std::string& fileName) 
		{
			std::string ex = File::getFileExtension(fileName);
			if(ex.compare("obj") == 0)
			{
				return loadMeshFromFile(fileName);
			}
			else if(ex.compare("sk") == 0)
			{
				return loadSkeletonFromFile(fileName);
			}else if(ex == "bvh")
			{
				return loadBVHFromFile(fileName);
			}
		}

		static Entity* loadMeshFromFile(const std::string& fileName) 
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
				RenderManager::getInstance()->add(renderer);
				return entity;
			}else
			{
				delete mesh;
				return NULL;
			}
		}

		static Entity* loadSkeletonFromFile(const std::string& fileName) 
		{
			Scene* scene = SceneManager::getInstance()->getCurrentScene();
			SkeletonTextFileLoader loader;
			Skeleton* sk = loader.loadFromFile(fileName);
			
			//bool created = loader.loadFromFile(fileName, mesh);
			if(sk != NULL){
				GLSkeletonRenderer* skeletonRenderer = new GLSkeletonRenderer(fileName);
				skeletonRenderer->setSkeleton(sk);

				Entity* entity = new Entity(fileName, scene);
				entity->setComponent(ComponentType::RENDER_COMPONENT, skeletonRenderer);
				entity->setComponent(ComponentType::SKELETON_COMPONENT, sk);
				RenderManager::getInstance()->add(skeletonRenderer);
				return entity;
			}else
			{
				return NULL;
			}
		}


		static Entity* loadBVHFromFile(const std::string& fileName) 
		{
			Scene* scene = SceneManager::getInstance()->getCurrentScene();
			BVH * bvh = new BVH();
			bvh->loadFromBVHFile(fileName);
			Skeleton* sk = new Skeleton(fileName);
			SkeletonConverter::convertFromBVHToSkeleton(bvh, sk);
			//bool created = loader.loadFromFile(fileName, mesh);
			if(sk != NULL){
				GLSkeletonRenderer* skeletonRenderer = new GLSkeletonRenderer(fileName);
				skeletonRenderer->setSkeleton(sk);

				Entity* entity = new Entity(fileName, scene);
				entity->setComponent(ComponentType::RENDER_COMPONENT, skeletonRenderer);
				entity->setComponent(ComponentType::SKELETON_COMPONENT, sk);
				RenderManager::getInstance()->add(skeletonRenderer);
				BVHSkeletonAnimator* animator = new BVHSkeletonAnimator(bvh, sk);
				AnimationManager::getInstance()->addIntoAnimatorList(animator);
				return entity;
			}else
			{
				return NULL;
			}
		}


	};

}
