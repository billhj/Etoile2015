/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ObjectRenderer.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>

namespace Etoile
{
	class ObjectRenderer
	{
	protected:
		std::string _name;
		bool _visible;
	public:
		ObjectRenderer(const std::string& name): _name(name), _visible(true)
		{
		}
		virtual void draw() = 0;
		void setVisible(bool visible){_visible = visible;}
		
	};
}
