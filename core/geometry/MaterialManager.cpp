/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MaterialManager.cpp
* @brief 
* @date 1/2/2011
*/

#include "MaterialManager.h"
#include "Material.h"
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

	MaterialManager::MaterialManager()
	{
	}

	unsigned int MaterialManager::addMaterial(Material* t)
	{
		int size = m_datas.size();
		m_datas.push_back(t);
		return size;
	}

	Material* MaterialManager::getMaterialByIndex(unsigned int idx)
	{
		if(idx < m_datas.size())
			return m_datas[idx];
		return NULL;
	}

	void MaterialManager::addMaterials(std::vector<Material*> resources)
	{
		for(unsigned int i = 0; i < resources.size(); ++i)
		{
			addMaterial(resources[i]);
		}
	}
}
