/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file RenderManager.cpp
* @brief 
* @date 1/2/2011
*/

#include "RenderManager.h"
#include "Scene.h"
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

	void RenderManager::clear()
	{
		for(unsigned int i = 0; i < m_renderObjList.size(); ++i)
		{
			delete m_renderObjList[i];
		}
		m_renderObjList.clear();
	}
	
	void RenderManager::preRender()
	{
		for(unsigned int i = 0; i < m_preRenderObjList.size(); ++i)
		{
			ObjectRenderer* obj = m_preRenderObjList[i];
			obj->draw();
		}
	}

	void RenderManager::renderOneFrame()
	{
		for(unsigned int i = 0; i < m_renderObjList.size(); ++i)
		{
			ObjectRenderer* obj = m_renderObjList[i];
			obj->draw();
		}
	}

	void RenderManager::PostRender()
	{
		for(unsigned int i = 0; i < m_postRenderObjList.size(); ++i)
		{
			ObjectRenderer* obj = m_postRenderObjList[i];
			obj->draw();
		}
	}

	void RenderManager::renderTexcoord()
	{
		for(unsigned int i = 0; i < m_renderObjList.size(); ++i)
		{
			ObjectRenderer* obj = m_renderObjList[i];
			obj->drawTexcoord();
		}
	}

	void RenderManager::addObjectRenderer(ObjectRenderer* obj)
	{
		m_renderObjList.push_back(obj);
	}

	void RenderManager::addPrelist(ObjectRenderer* obj)
	{
		m_preRenderObjList.push_back(obj);
	}

	void RenderManager::addPostlist(ObjectRenderer* obj)
	{
		m_postRenderObjList.push_back(obj);
	}

	void RenderManager::updateObjectRendererList(Scene* scene)
	{
		m_renderObjList.clear();
		const std::vector<Entity*>& entities = scene->getEntities();
		for(unsigned int i = 0; i < entities.size(); ++i)
		{
			Entity* entity = entities[i];
			ObjectRenderer* ro = entity->getObjectRenderer();
			this->addObjectRenderer(ro);
		}
	}

	void RenderManager::addIntoObjectRendererList(ObjectRenderer* cobj)
	{
		if(m_renderObjList.size() > 0)
		{
			int offset = sizeof(cobj - m_renderObjList[0]) / sizeof(ObjectRenderer);
			if(cobj == m_renderObjList[offset - 1])
			{
				return;
			}
		}
		
		/*for(unsigned int i = 0; i < m_renderObjList.size(); ++i)
		{
			ObjectRenderer* obj = m_renderObjList[i];
			if(cobj == obj)
			{
				return;
			}
		}*/
		m_renderObjList.push_back(cobj);
	}

	void RenderManager::removeFromObjectRendererList(ObjectRenderer* cobj)
	{
		/*for(unsigned int i = 0; i < m_renderObjList.size(); ++i)
		{
			ObjectRenderer* obj = m_renderObjList[i];
			if(cobj == obj)
			{
				m_renderObjList.erase(m_renderObjList.begin()+i);
				return;
			}
		}*/
		if(m_renderObjList.size() > 0)
		{
			int offset = sizeof(cobj - m_renderObjList[0]) / sizeof(ObjectRenderer);
			if(cobj == m_renderObjList[offset])
			{
				m_renderObjList.erase(m_renderObjList.begin()+offset);
			}
		}
	}

	void RenderManager::removeFromPreList(ObjectRenderer* cobj)
	{
		/*for(unsigned int i = 0; i < m_preRenderObjList.size(); ++i)
		{
			ObjectRenderer* obj = m_preRenderObjList[i];
			if(cobj == obj)
			{
				m_preRenderObjList.erase(m_preRenderObjList.begin()+i);
				return;
			}
		}*/
		if(m_preRenderObjList.size() > 0)
		{
			int offset = sizeof(cobj - m_preRenderObjList[0]) / sizeof(ObjectRenderer);
			if(cobj == m_preRenderObjList[offset])
			{
				m_preRenderObjList.erase(m_preRenderObjList.begin()+offset);
			}
		}
	}

	void RenderManager::removeFromPostList(ObjectRenderer* cobj)
	{
		/*for(unsigned int i = 0; i < m_postRenderObjList.size(); ++i)
		{
			ObjectRenderer* obj = m_postRenderObjList[i];
			if(cobj == obj)
			{
				m_postRenderObjList.erase(m_postRenderObjList.begin()+i);
				return;
			}
		}*/
		if(m_postRenderObjList.size() > 0)
		{
			int offset = sizeof(cobj - m_postRenderObjList[0]) / sizeof(ObjectRenderer);
			if(cobj == m_postRenderObjList[offset])
			{
				m_postRenderObjList.erase(m_postRenderObjList.begin()+offset);
			}
		}
	}
}
