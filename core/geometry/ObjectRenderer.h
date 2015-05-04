/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ObjectRenderer.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include "Component.h"

namespace Etoile
{
	class ObjectRenderer : public Component
	{
	protected:
		std::string _name;
	public:
		ObjectRenderer(const std::string& name): _name(name), Component(RENDER_COMPONENT)
		{
		}
		virtual void draw() = 0;
		
	};
}
