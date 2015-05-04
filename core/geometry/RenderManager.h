/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file RenderManager.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <vector>

namespace Etoile
{
	class ObjectRenderer;
	class Scene;
	class RenderManager
	{
	
	public:
		RenderManager(const std::string& name);
		virtual ~RenderManager();
		virtual void clear();
	
		virtual void renderOneFrame();

		void addObjectRenderer(ObjectRenderer* obj);
		void updateObjectRendererList(Scene* scene);
		void addIntoObjectRendererList(ObjectRenderer* obj);
		void removeFromObjectRendererList(ObjectRenderer* obj);
	protected:
		std::string m_name;
		std::vector<ObjectRenderer*> m_renderObjList;
	};
}
