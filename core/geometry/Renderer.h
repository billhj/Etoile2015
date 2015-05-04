/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Renderer.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <vector>

namespace Etoile
{
	class RenderObject;
	class Scene;
	class Renderer
	{
	
	public:
		Renderer(const std::string& name);
		virtual ~Renderer();
		virtual void clear();
	
		virtual void renderOneFrame();

		void addRenderObject(RenderObject* obj);
		void updateRenderObjectList(Scene* scene);
		void addIntoRenderObjectList(RenderObject* obj);
		void removeFromRenderObjectList(RenderObject* obj);
	protected:
		std::string m_name;
		std::vector<RenderObject*> m_renderObjList;
	};
}
