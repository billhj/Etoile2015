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
		RenderManager();
	public:
		static RenderManager* getInstance()
		{
			static RenderManager manager;
			return &manager;
		}
		
		virtual ~RenderManager();
		virtual void clear();
	
		virtual void preRender();
		virtual void renderOneFrame();
		virtual void PostRender();

		virtual void renderTexcoordPicking();

		
		void updateObjectRendererList(Scene* scene);
		void addIntoObjectRendererList(ObjectRenderer* obj);
		

		void addObjectRenderer(ObjectRenderer* obj);
		void addPrelist(ObjectRenderer* obj);
		void addPostlist(ObjectRenderer* obj);
		void removeFromObjectRendererList(ObjectRenderer* obj);
		void removeFromPreList(ObjectRenderer* obj);
		void removeFromPostList(ObjectRenderer* obj);

		void clearRenderList(){m_renderObjList.clear();}
		void clearPreList(){m_preRenderObjList.clear();}
		void clearPostList(){m_postRenderObjList.clear();}

	protected:
		std::string m_name;
		std::vector<ObjectRenderer*> m_renderObjList;
		std::vector<ObjectRenderer*> m_preRenderObjList;
		std::vector<ObjectRenderer*> m_postRenderObjList;
	};
}
