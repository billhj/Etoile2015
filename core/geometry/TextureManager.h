/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file TextureManager.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "ResourceManager.h"

namespace Etoile
{
	class Texture;
	class TextureCreator
	{
	public:
		virtual Texture* createFromFile(const std::string& ref) = 0;
	};

	class TextureManager : public ResourceManager<Texture>
	{
	private:
		TextureManager();
	protected:
		TextureCreator* p_creator;
	public:
		static TextureManager* getInstance()
		{
			static TextureManager manager;
			return &manager;
		}

		void setTextureCreator(TextureCreator* creator)
		{
			p_creator = creator;
		}

		TextureCreator* getCurrentTextureCreator(){return p_creator;}
	};

}
