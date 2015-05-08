/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file TextureManager.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "ResourceManager.h"

namespace Etoile
{
	class Texture;
	class TextureManager : public ResourceManager<Texture>
	{
		TextureManager();
	public:
		static TextureManager* getInstance()
		{
			static TextureManager manager;
			return &manager;
		}

		unsigned int addTexture(Texture* t);
		Texture* getTextureByName(const std::string& name);
		Texture* getTextureByIndex(unsigned int idx);
		void addTextures(std::vector<Texture*> resources);
	};

}
