/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GpuProgram.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include <map>
#include "GpuProgramManager.h"

namespace Etoile
{

	class GpuProgram
	{
	protected:
		std::string m_name;
	public:
		GpuProgram(const std::string& name): m_name(name)
		{
			GpuProgramManager::getInstance()->add(this);
		}

		GpuProgram(GpuProgram& g)
		{
			m_name = g.m_name+"_copy";
			GpuProgramManager::getInstance()->add(this);
		}
		~GpuProgram(){}
		const std::string getName(){return m_name;}
		void setName(const std::string& name){m_name = name;}
		virtual void reCompile() = 0;
		virtual void use() = 0;
		virtual void unUse() = 0;
		virtual void release() = 0;
	};

}
