/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file RenderManager.cpp
* @brief 
* @date 1/2/2011
*/

#include "RenderManager.h"
#include "ObjectRenderer.h"

namespace Etoile
{
	RenderManager::RenderManager()
	{
	}

	RenderManager::~RenderManager()
	{
		
		clear();
	}



	void RenderManager::renderOneFrame()
	{
		for(unsigned int i = 0; i < m_datas.size(); ++i)
		{
			ObjectRenderer* obj = m_datas[i];
			obj->draw();
		}
	}


}
