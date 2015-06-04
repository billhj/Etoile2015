/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Skeleton.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include <iostream>
#include <Eigen/Dense>

namespace Etoile
{

	struct IKChain
	{

		struct Dim
		{
			int m_lastIdx;
			int m_idx;
			Eigen::Vector3f m_axis;
			Eigen::Vector2f m_anglelimites;
			float m_value;
		};

		struct Joint
		{
			Joint(IKChain* sk, int parent, int dof, const std::string& name);
			std::string m_name;
			int m_index;
			int m_index_parent;
			IKChain* p_owner;
			int m_dof;
			std::vector<Dim> m_dims;
		};

		IKChain(const std::string& name) : m_name(name)
		{
		
		}

		void updateJoint(int idx)
		{
			Joint* current = m_joints[idx];
			for(unsigned int i = 0; i < current->m_dims.size(); ++i)
			{
				Dim& dim = current->m_dims[i];
				if(dim.m_lastIdx >= 0)
				{
					m_globalPositions[dim.m_idx] = m_globalPositions[dim.m_lastIdx] + m_globalOrientations[dim.m_lastIdx] * m_localTranslations[dim.m_idx];
					m_globalOrientations[dim.m_idx] = m_globalOrientations[dim.m_lastIdx] * m_localRotations[dim.m_idx];	
				}
				else
				{
					m_globalPositions[dim.m_idx] = m_localTranslations[dim.m_idx];
					m_globalOrientations[dim.m_idx] = m_localRotations[dim.m_idx];
				}
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
		std::vector<Eigen::Matrix3f> m_localRotations;
		std::vector<Eigen::Matrix3f> m_globalOrientations;
		std::vector<Eigen::Vector3f> m_localTranslations;
		std::vector<Eigen::Vector3f> m_globalPositions;
		std::string m_name;


		bool loadFromFile(const std::string& fileName);
		void read(std::istream& in);
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};
}
