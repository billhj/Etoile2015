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
#include <fstream>
//#include <Eigen/StdVector>

namespace Etoile
{
	
	typedef Eigen::MatrixXd MatrixX_;
	typedef Eigen::Matrix3d Matrix3_;
	typedef Eigen::VectorXd VectorX_;
	typedef Eigen::Vector3d Vector3_;
	typedef Eigen::Vector4d Vector4_;
	typedef Eigen::Vector2d Vector2_;
	typedef Eigen::AngleAxisd AngleAxis_;

//	
//EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(MatrixX_)
//EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Matrix3_)
//EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(VectorX_)
//EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector3_)
//EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector2_)

	struct IKChain
	{

		struct Dim
		{
			int m_lastIdx;
			int m_idx;
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

		IKChain(const std::string& name="") : m_name(name)
		{
		
		}

		void reset();
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

		void output(const std::string& name)
		{
			std::ofstream out;
			out.open(name);
			for(unsigned int j = 0; j < m_values.size(); ++j)
			{
				out<<m_values[j]<<", ";
			}
			out<<std::endl;
			for(unsigned int j = 0; j < m_anglelimites.size(); ++j)
			{
				out<<m_anglelimites[j][0]<<", ";
			}
			out<<std::endl;
			for(unsigned int j = 0; j < m_anglelimites.size(); ++j)
			{
				out<<m_anglelimites[j][1]<<", ";
			}
			out<<std::endl;
			out.close();
		}

		std::vector<Joint*> m_joints;
		std::vector<Matrix3_, Eigen::aligned_allocator<Matrix3_> > m_localRotations;
		std::vector<Matrix3_, Eigen::aligned_allocator<Matrix3_> > m_globalOrientations;
		std::vector<Vector3_, Eigen::aligned_allocator<Vector3_> > m_localTranslations;
		std::vector<Vector3_, Eigen::aligned_allocator<Vector3_> > m_globalPositions;
		std::string m_name;

		std::vector<Vector3_, Eigen::aligned_allocator<Vector3_> > m_axis;
		std::vector<Vector2_, Eigen::aligned_allocator<Vector2_> > m_anglelimites;
		std::vector<double> m_values;
		std::vector<double> m_drLimits_positive;
		std::vector<double> m_drLimits_negative;

		std::vector<double> m_average_values;


		bool loadFromFile(const std::string& fileName);
		void read(std::istream& in);
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};
}


struct IKTree : public IKChain
{
std::vector<int> m_end_effector_index;
};