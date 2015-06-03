/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Skeleton.cpp
* @brief 
* @date 1/2/2011
*/

#include "Skeleton.h"

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
}
