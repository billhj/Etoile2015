/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GpuProgramManager.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "ResourceManager.h"

namespace Etoile
{
	class GpuProgram;
	class GpuProgramManager : public ResourceManager<GpuProgram>
	{
		GpuProgramManager();
	public:
		static GpuProgramManager* getInstance()
		{
			static GpuProgramManager manager;
			return &manager;
		}

		unsigned int addGpuProgram(GpuProgram* t);
		//GpuProgram* getGpuProgramByName(const std::string& name);
		GpuProgram* getGpuProgramByIndex(unsigned int idx);
		void addGpuPrograms(std::vector<GpuProgram*> resources);
	};

}
