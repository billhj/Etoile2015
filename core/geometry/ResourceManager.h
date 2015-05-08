/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file ResourceManager.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include <assert.h>
#include <vector>
#include <map>

namespace Etoile
{
	template <class DataType>
	class ResourceManager
	{
	public:
		DataType* getByIndex(unsigned int idx)
		{
			assert(idx < _datas.size());
			return _datas[idx];
		}


		DataType* getByName(const std::string& name)
		{
			std::map<std::string, int>::iterator itor = m_names_idx.find(name);
			if(itor != m_names_idx.end())
			{
				return m_datas[(*itor).second];
			}
			return NULL;
		}

		int getIndex(const std::string& name)
		{
			std::map<std::string, int>::iterator itor = m_names_idx.find(name);
			if(itor != m_names_idx.end())
			{
				return (*itor).second;
			}
			return -1;
		}

		unsigned int add(const std::string& name, DataType* data)
		{
			std::map<std::string, int>::iterator itor = m_names_idx.find(name);
			if(itor != m_names_idx.end())
			{
				DataType* olddata = m_datas[(*itor).second];
				int idx = (*itor).second;
				m_datas[idx] = data;
				delete olddata;
				return idx;
			}
			else
			{
				int idx = m_datas.size();
				m_datas.push_back(data);
				m_names_idx[name] = idx;
				return idx;
			}
		}

	protected:
		std::vector<DataType*> m_datas;
		std::map<std::string, int> m_names_idx;
	};
}


#endif //RESOURCE_MANAGER_H