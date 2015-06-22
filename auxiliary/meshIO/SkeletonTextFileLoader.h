/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file OBJSimpleMeshLoader.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#define NOMINMAX
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <istream>
#include <deque>
#include <map>
#include "geometry/Skeleton.h"

namespace Etoile
{
	class SkeletonTextFileLoader
	{
		std::string m_path;
		std::string m_fileName;
	public:
		SkeletonTextFileLoader(){}
		~SkeletonTextFileLoader(){}
		Skeleton* loadFromFile(const std::string& fileName)
		{
			m_fileName = fileName;
			std::fstream in(fileName.c_str(), std::ios_base::in );

			if (!in.is_open() || !in.good())
			{
				std::cerr << "[SkeletonTextFileLoader] : cannot not open file "
					<< fileName
					<< std::endl;
				return false;
			}

			{
#if defined(WIN32)
				std::string::size_type dot = fileName.find_last_of("\\/");
#else
				std::string::size_type dot = fileName.rfind("/");
#endif
				m_path = (dot == std::string::npos)
					? "./"
					: std::string(fileName.substr(0,dot+1));
			}

			Skeleton* result = read(in);

			in.close();
			return result;
		}

	protected:
		Skeleton* read(std::istream& in)
		{
			Skeleton* sk = new Skeleton(m_fileName);
			std::string line;
			int lineNb = 0;
			while( in && !in.eof() )
			{
				std::getline(in,line);
				if ( in.bad() ){
					std::cout << "  Warning! Could not read file properly!"<<std::endl;
					return false;
				}

				if ( line.size() == 0 || line[0] == '#' || isspace(line[0]) || line.empty() ) {
					continue;
				}

				std::stringstream stream(line);
				try
				{
					std::string name;
					stream >> name;
					int idxP;
					stream >> idxP;
					int dof;
					stream >> dof;
					Joint* j = new Joint(sk, idxP, dof, name);
					stream >> sk->m_localTranslations[j->m_index][0];
					stream >> sk->m_localTranslations[j->m_index][1];
					stream >> sk->m_localTranslations[j->m_index][2];
					for(int i = 0; i < dof; ++i)
					{
						std::getline(in,line);
						std::stringstream stream(line);
						stream >> j->m_axis[i][0];
						stream >> j->m_axis[i][1];
						stream >> j->m_axis[i][2];
						stream >> j->m_anglelimites[i][0];
						stream >> j->m_anglelimites[i][1];
						++lineNb;
					}
				
				}catch(exception& e)
				{
					std::cout<<"SkeletonTextFileLoader: exception "<< lineNb<<" name "<<std::endl;
					continue;
				}
				++lineNb;
			}
			sk->update();
			if(sk->m_joints.size()<1)
			{
				delete sk;
				return NULL;
			}
			return sk;
		}


	};

}
