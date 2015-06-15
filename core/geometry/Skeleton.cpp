/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Skeleton.cpp
* @brief 
* @date 1/2/2011
*/

#include "Skeleton.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
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

	Joint::Joint(Skeleton* sk, int parent, const std::string& name)
	{
		p_owner = sk;
		m_index = sk->m_joints.size();
		p_owner->m_name_index[name] = m_index;
		sk->m_joints.push_back(this);
		m_index_parent = parent;
		m_name = name;
	}

	void Joint::init(int degree)
	{
		if(degree == 1)
		{
			m_dof = 1;
			m_axis.push_back(Vec3f(1,0,0));
			m_anglelimites.push_back(Vec2f(-3.14,3.14));
		}else
		{
			m_dof = 3;
			m_axis.push_back(Vec3f(0,0,1));
			m_axis.push_back(Vec3f(0,1,0));
			m_axis.push_back(Vec3f(1,0,0));
			m_anglelimites.push_back(Vec2f(-3.14,3.14));
			m_anglelimites.push_back(Vec2f(-3.14,3.14));
			m_anglelimites.push_back(Vec2f(-3.14,3.14));
		}
	}


	bool Skeleton::loadFromFile(const std::string& fileName)
	{
		m_path = fileName;
		std::fstream in(fileName.c_str(), std::ios_base::in );

		if (!in.is_open() || !in.good())
		{
			std::cerr << "[Skeleton] : cannot not open file "
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

		read(in);
		in.close();
		return true;
	}

	void Skeleton::read(std::istream& in)
	{
		std::string line;
		int lineNb = 0;
		while( in && !in.eof() )
		{
			std::getline(in,line);
			if ( in.bad() ){
				std::cout << "  Warning! Could not read file properly!"<<std::endl;
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
				float x,y,z;
				stream >> x;
				stream >> y;
				stream >> z;
				Joint* j = new Joint(this, idxP, name);
				m_localTranslations.push_back(Vec3f(x,y,z));
				m_globalPositions.push_back(Vec3f());
				m_localRotations.push_back(Quaternionf());
				m_globalOrientations.push_back(Quaternionf());

			}catch(exception& e)
			{
				std::cout<<"SkeletonTextFileLoader: exception "<< lineNb<<" name "<<std::endl;
				continue;
			}
			++lineNb;
		}
		update();
	}
}
