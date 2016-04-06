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


				writeoutSkeletonFile(fileName+"_.csv", sk, bvh);
				return entity;
			}else
			{
				return NULL;
			}
		}

		static float degreeToRadian(float v)
		{
			return v * 3.14159265 / 180;
		}

		static void writeoutSkeletonFile(const std::string& fileName, Skeleton* sk, BVH* bvh)
		{
			std::vector<std::string> titlename;
			std::vector<std::vector<float>> values;

			for(unsigned int m_frameIndex = 0; m_frameIndex < bvh->m_frameNb; ++m_frameIndex)
			{
				std::vector<float> value;
				Frame& frame = bvh->m_frames[m_frameIndex];
				for(unsigned int i = 0; i < bvh->m_joints.size(); ++i)
				{
					BVH::Joint* joint = bvh->m_joints[i];
					Joint* jsk = sk->m_joints[i];
					Quaternionf q;
					for(unsigned int j = 0; j < joint->m_dims.size(); ++j)
					{
						BVH::Dim& dim = joint->m_dims[j];
						float v = frame.m_values[dim.m_index];
						if(dim.m_name == "Xposition")
						{
							sk->m_localTranslations[i][0] = v / 100.0;
						}
						else if(dim.m_name == "Yposition")
						{
							sk->m_localTranslations[i][1] = v / 100.0;
						}
						else if(dim.m_name == "Zposition")
						{
							sk->m_localTranslations[i][2] = v / 100.0;
						}
						else if(dim.m_name == "Zrotation")
						{
							q = q * Quaternionf(Vec3f(0,0,1),degreeToRadian(v));
						}
						else if(dim.m_name == "Xrotation")
						{
							q = q * Quaternionf(Vec3f(1,0,0),degreeToRadian(v));
						}
						else if(dim.m_name == "Yrotation")
						{
							q = q * Quaternionf(Vec3f(0,1,0),degreeToRadian(v));
						}
					}
					sk->m_localRotations[i] = q;
					sk->updateJoint(i);
					Vec3f pos = sk->m_globalPositions[i];
					Vec3f localR =  q.axis() * q.angle();
					if(m_frameIndex == 0)
					{
						titlename.push_back(sk->m_joints[i]->m_name + "_x");
						titlename.push_back(sk->m_joints[i]->m_name + "_y");
						titlename.push_back(sk->m_joints[i]->m_name + "_z");
						titlename.push_back(sk->m_joints[i]->m_name + "_rx");
						titlename.push_back(sk->m_joints[i]->m_name + "_ry");
						titlename.push_back(sk->m_joints[i]->m_name + "_rz");
					}
					value.push_back(pos.x());
					value.push_back(pos.y());
					value.push_back(pos.z());
					value.push_back(localR.x());
					value.push_back(localR.y());
					value.push_back(localR.z());
				}

				values.push_back(value);
				sk->update();
			}


			std::ofstream out;
			out.open(fileName);

			for(unsigned int i = 0; i < titlename.size(); ++i)
			{
				out<<titlename[i]<<", ";
			}
			out<<std::endl;

			for(unsigned int i = 0; i < values.size(); ++i)
			{
				
				for(unsigned int j = 0; j < values[i].size(); ++j)
				{
					out<<values[i][j]<<", ";
				}
				out<<std::endl;
			}

			out.close();
			std::cout<<fileName<<"writing finished"<<std::endl;
		}




	};

}
