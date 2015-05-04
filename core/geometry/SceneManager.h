/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SceneManager.cpp
* @brief 
* @date 1/2/2011
*/

#pragma once

#include "Scene.h"

namespace Etoile
{
	class SceneManager
	{
	public:
		SceneManager(void);
		~SceneManager(void);
		void addScene(Scene*);
		std::vector<Scene*>& getSceneList(){return m_scenes;}
		const std::vector<Scene*>& getSceneList() const {return m_scenes;}
		void setCurrrentScene(Scene* current);
		Scene* getCurrentScene();
	private:
		std::vector<Scene*> m_scenes;
		Scene* p_current;
	};

}