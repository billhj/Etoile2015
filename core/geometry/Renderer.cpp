/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Renderer.cpp
* @brief 
* @date 1/2/2011
*/

#include "Renderer.h"
#include "Scene.h"
#include "RenderObject.h"

namespace Etoile
{
	Renderer::Renderer(const std::string& name): m_name(name)
	{
	}

	Renderer::~Renderer()
	{
		
		clear();
	}

	void Renderer::clear()
	{
		for(unsigned int i = 0; i < m_renderObjList.size(); ++i)
		{
			delete m_renderObjList[i];
		}
		m_renderObjList.clear();
	}
	
	void Renderer::renderOneFrame()
	{
		for(unsigned int i = 0; i < m_renderObjList.size(); ++i)
		{
			RenderObject* obj = m_renderObjList[i];
			obj->draw();
		}
	}

	void Renderer::addRenderObject(RenderObject* obj)
	{
		m_renderObjList.push_back(obj);
	}

	void Renderer::updateRenderObjectList(Scene* scene)
	{
		m_renderObjList.clear();
		const std::vector<Entity*>& entities = scene->getEntities();
		for(unsigned int i = 0; i < entities.size(); ++i)
		{
			Entity* entity = entities[i];
			RenderObject* ro = entity->getRenderObject();
			this->addRenderObject(ro);
		}
	}

	void Renderer::addIntoRenderObjectList(RenderObject* cobj)
	{
		for(unsigned int i = 0; i < m_renderObjList.size(); ++i)
		{
			RenderObject* obj = m_renderObjList[i];
			if(cobj == obj)
			{
				return;
			}
		}
		m_renderObjList.push_back(cobj);
	}

	void Renderer::removeFromRenderObjectList(RenderObject* cobj)
	{
		for(unsigned int i = 0; i < m_renderObjList.size(); ++i)
		{
			RenderObject* obj = m_renderObjList[i];
			if(cobj == obj)
			{
				m_renderObjList.erase(m_renderObjList.begin()+i);
				return;
			}
		}
	}

	
}
