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
#include <string>

namespace Etoile
{
	template <class DataType>
	class ResourceManager
	{
	public:
		DataType* getByIndex(unsigned int idx)
		{
			assert(idx < m_datas.size());
			return m_datas[idx];
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
				int idx = (*itor).second;
				DataType* olddata = m_datas[idx];
				if(olddata != data)
				{
					m_datas[idx] = data;
					delete olddata;
				}
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

		void remove(const std::string& name, bool deleteData = true)
		{
			std::map<std::string, int>::iterator itor = m_names_idx.find(name);
			if(itor != m_names_idx.end())
			{
				int idx = (*itor).second;
				DataType* olddata = m_datas[idx];
				if(deleteData)
				{
					delete olddata;
					m_datas[idx] = NULL;
				}
				m_names_idx.erase(itor);
			}
		}

		std::vector<DataType*>& getDataList(){return m_datas;}
		const std::vector<DataType*>& getConstDataList() const {return m_datas;}
		void renameElement(const std::string& oldName, const std::string& newName)
		{
			int idx = getIndex(oldName);
			remove(oldName, false);
			m_names_idx[newName] = idx;
		}

	protected:
		std::vector<DataType*> m_datas;
		std::map<std::string, int> m_names_idx;
	};
}


#endif //RESOURCE_MANAGER_H