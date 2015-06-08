/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SceneManager.cpp
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "ResourceManager.h"
#include "Scene.h"

namespace Etoile
{
	class SceneManager : public ResourceManager<Scene>
	{
		SceneManager(void);
	public:
		static SceneManager* getInstance()
		{
			static SceneManager manager;
			return &manager;
		}
		~SceneManager(void);
		void setCurrrentScene(Scene* current);
		Scene* getCurrentScene();
	private:
		Scene* p_current;
	};

}