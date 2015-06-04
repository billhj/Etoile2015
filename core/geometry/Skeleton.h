/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Skeleton.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "math/MathHead.h"
#include "Component.h"

namespace Etoile
{
	struct Skeleton;
	struct Joint
	{
		Joint(Skeleton* sk, int parent, const std::string& name);
		void init(int degree = 3);
		std::string m_name;
		int m_index;
		int m_index_parent;
		Skeleton* p_owner;
		int m_dof;
		std::vector<Vec3f> m_axis;
		std::vector<Vec2f> m_anglelimites;
		std::vector<float> m_values;
	};


	struct Skeleton : public Component
	{
		Skeleton(const std::string& name) : Component(SKELETON_COMPONENT), m_name(name)
		{
		
		}
		void updateJoint(int idx)
		{
			int parentId = m_joints[idx]->m_index_parent;
			if(parentId < 0)
			{
				m_globalPositions[idx] = m_localTranslations[idx];
				m_globalOrientations[idx] = m_localRotations[idx];
			}
			else
			{
				m_globalPositions[idx] = m_globalPositions[parentId] + m_globalOrientations[parentId] * m_localTranslations[idx];
				m_globalOrientations[idx] = m_globalOrientations[parentId] * m_localRotations[idx];
			}
		}

		void update()
		{
			for(unsigned int i = 0; i < m_joints.size(); ++i)
			{
				updateJoint(i);
			}
		}

		std::vector<Joint*> m_joints;
		std::vector<Quaternionf> m_localRotations;
		std::vector<Quaternionf> m_globalOrientations;
		std::vector<Vec3f> m_localTranslations;
		std::vector<Vec3f> m_globalPositions;
		std::string m_name;
	};
}