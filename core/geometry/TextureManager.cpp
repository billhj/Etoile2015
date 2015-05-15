/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file TextureManager.cpp
* @brief 
* @date 1/2/2011
*/

#include "TextureManager.h"
#include "Texture.h"
#include <assert.h>

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{

	TextureManager::TextureManager()
	{
	}

	unsigned int TextureManager::addTexture(Texture* t)
	{
		int size = m_datas.size();
		m_datas.push_back(t);
		return size;
	}

	
	Texture* TextureManager::getTextureByIndex(unsigned int idx)
	{
		if(idx < m_datas.size())
			return m_datas[idx];
		return NULL;
	}

	void TextureManager::addTextures(std::vector<Texture*> resources)
	{
		for(unsigned int i = 0; i < resources.size(); ++i)
		{
			addTexture(resources[i]);
		}
	}
}
