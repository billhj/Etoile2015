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
#include "geometry/ResourceManager.h"

namespace Etoile
{
	class ObjectRenderer;
	class RenderManager : public ResourceManager<ObjectRenderer>
	{
		RenderManager();
	public:
		static RenderManager* getInstance()
		{
			static RenderManager manager;
			return &manager;
		}
		
		virtual ~RenderManager();
	
		virtual void renderOneFrame();

	protected:
		std::string m_name;

		/*std::vector<ObjectRenderer*> m_preRenderObjList;
		std::vector<ObjectRenderer*> m_postRenderObjList;*/
	};
}
