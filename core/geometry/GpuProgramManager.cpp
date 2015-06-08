/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GpuProgramManager.cpp
* @brief 
* @date 1/2/2011
*/

#include "GpuProgramManager.h"
#include "GpuProgram.h"
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

	GpuProgramManager::GpuProgramManager() : ResourceManager<GpuProgram>()
	{
	}

	unsigned int GpuProgramManager::addGpuProgram(GpuProgram* t)
	{
		return add(t);
	}

	/*GpuProgram* GpuProgramManager::getGpuProgramByName(const std::string& name)
	{
		return (GpuProgram*)getByName(name);
	}*/

	GpuProgram* GpuProgramManager::getGpuProgramByIndex(unsigned int idx)
	{
		return (GpuProgram*)get(idx);
	}

	void GpuProgramManager::addGpuPrograms(std::vector<GpuProgram*> resources)
	{
		for(unsigned int i = 0; i < resources.size(); ++i)
		{
			addGpuProgram(resources[i]);
		}
	}
}
