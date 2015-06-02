/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Skeleton.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>

namespace Etoile
{
	class Skeleton;
	class Transform;
	struct Joint
	{
		Joint(Skeleton* sk, int parent, const std::string& name);

		std::string m_name;
		int m_index;
		int m_index_parent;
		Skeleton* p_owner;
		int m_dof;
	};


	struct Skeleton
	{
		/*void updateJoint(int idx)
		{
			int parentId = m_joints[idx]->m_index_parent;
			m_globalTransform[idx] = m_globalTransform[parentId] * m_localTransform[idx];
		}*/

		std::vector<Joint*> m_joints;
		std::vector<Transform> m_localTransform;
		std::vector<Transform> m_globalTransform;
	};
}
