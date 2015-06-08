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
#include <exception>
#include <vector>
#include <map>
#include <string>

namespace Etoile
{
	template <class DataType>
	class ResourceManager
	{
	public:
		DataType* get(unsigned int idx)
		{
			assert(idx < m_datas.size());
			return m_datas[idx];
		}

		void clear()
		{
			for(unsigned int i = 0; i < m_datas.size(); ++i)
			{
				delete m_datas[i];
			}
			m_datas.clear();
		}

		/***
		*	find data by memory adresse
		*/
		int find(DataType* data)
		{
			if(m_datas.size() > 0)
			{
				int offset = sizeof(data - m_datas[0]) / sizeof(DataType);
				if(offset > m_datas.size() || offset < 0) return -1;
				if(data == m_datas[offset])
				{
					return offset;
				}
			}
			return -1;
			/*try
			{
				int offset = sizeof(data - m_datas[0]) / sizeof(DataType);
				if(data == m_datas[offset])
				{
					return offset;
				}
			}
			catch(std::exception& ex)
			{
				return -1;
			}*/
		}

		/***
		*	add data by checking memory adresse
		*/
		unsigned int add(DataType* data)
		{
			int index = find(data);
			if(index >= 0) return index;
			index = m_datas.size();
			m_datas.push_back(data);
			return index;
		}

		void add(const std::vector<DataType*>& resources)
		{
			for(unsigned int i = 0; i < resources.size(); ++i)
			{
				add(resources[i]);
			}
		}

		/***
		*	remove data by checking memory adresse
		*/
		void remove(DataType* data)
		{
			int index = find(data);
			if(index >= 0) m_datas.erase(m_datas.begin() + index);
		}

		std::vector<DataType*>& getDataList(){return m_datas;}
		const std::vector<DataType*>& getConstDataList() const {return m_datas;}
	protected:
		std::vector<DataType*> m_datas;
	};
}


#endif //RESOURCE_MANAGER_H