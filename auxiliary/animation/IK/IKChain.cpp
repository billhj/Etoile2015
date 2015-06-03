/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Skeleton.cpp
* @brief 
* @date 1/2/2011
*/

#include "IKChain.h"

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

	IKChain::Joint::Joint(IKChain* sk, int parent, int dof, const std::string& name)
	{
		p_owner = sk;
		m_index = sk->m_joints.size();
		sk->m_joints.push_back(this);
		m_index_parent = parent;
		m_name = name;
		m_dof = dof;

		m_dims.resize(m_dof);
		for(int i = 0; i < m_dof; ++i)
		{
			m_dims[i].m_anglelimites = Eigen::Vector2f(-3.14,3.14);
			m_dims[i].m_idx = sk->m_localRotations.size();
			if(i == 0)
			{
				m_dims[i].m_lastIdx = sk->m_joints[parent]->m_dims.back().m_idx;
			}
			sk->m_localRotations.push_back(Eigen::Matrix3f());
			sk->m_globalOrientations.push_back(Eigen::Matrix3f());
			sk->m_localTranslations.push_back(Eigen::Vector3f());
			sk->m_globalPositions.push_back(Eigen::Vector3f());
		}
	}

	
}
