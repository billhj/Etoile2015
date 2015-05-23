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
	class TextureManager
	{
	private:
		TextureManager();
	protected:
		std::vector<Texture*> m_datas;
		
	public:
		static TextureManager* getInstance()
		{
			static TextureManager manager;
			return &manager;
		}

		unsigned int addTexture(Texture* t);
		Texture* getTextureByIndex(unsigned int idx);
		void addTextures(std::vector<Texture*> resources);
		const std::vector<Texture*>& getTextures();
	};

}
