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
}
