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
		p_creator = NULL;
	}
}
