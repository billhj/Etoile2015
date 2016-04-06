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


#ifndef X_
#define X_
	typedef Eigen::MatrixXd MatrixX_;
	typedef Eigen::Matrix3d Matrix3_;
	typedef Eigen::VectorXd VectorX_;
	typedef Eigen::Vector3d Vector3_;
	typedef Eigen::Vector4d Vector4_;
	typedef Eigen::Vector2d Vector2_;
	typedef Eigen::AngleAxisd AngleAxis_;
#endif

namespace Etoile
{


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
		void resetValues();

		void updateDimLocalRotation(int idx)
		{
			m_dim_localRotations[idx] = AngleAxis_(m_dim_values[idx], m_dim_axis[idx]);	
		}

		void updateDim(int idx)
		{
			updateDimLocalRotation(idx);
			Dim* dim = m_dims[idx];
			if(dim->m_lastIdx >= 0)
			{
				m_dim_globalPositions[dim->m_idx] = m_dim_globalPositions[dim->m_lastIdx] + m_dim_globalOrientations[dim->m_lastIdx] * m_dim_localTranslations[dim->m_idx];
				m_dim_globalOrientations[dim->m_idx] = m_dim_globalOrientations[dim->m_lastIdx] * m_dim_localRotations[dim->m_idx];	
			}
			else
			{
				m_dim_globalPositions[dim->m_idx] = m_dim_localTranslations[dim->m_idx];
				m_dim_globalOrientations[dim->m_idx] = m_dim_localRotations[dim->m_idx];
			}
		}

		void updateAllDims()
		{
			for(unsigned int i = 0; i < m_dims.size(); ++i)
			{
				updateDim(i);
			}
		}

		void updateJoint(int idx)
		{
			Joint* current = m_joints[idx];
			Matrix3_ temp = Matrix3_::Identity();
			for(unsigned int i = 0; i < current->m_dims.size(); ++i)
			{
				Dim& dim = current->m_dims[i];
				temp = temp * m_dim_localRotations[dim.m_idx];	
			}
			m_joint_localRotations[idx] = temp;
			int last = current->m_dof - 1;
			m_joint_globalOrientations[idx] = m_dim_globalOrientations[current->m_dims[last].m_idx];
			m_joint_globalPositions[idx] = m_dim_globalPositions[current->m_dims[last].m_idx];
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
			for(unsigned int j = 0; j < m_dim_values.size(); ++j)
			{
				out<<m_dim_values[j]<<", ";
			}
			out<<std::endl;
			for(unsigned int j = 0; j < m_dim_anglelimites.size(); ++j)
			{
				out<<m_dim_anglelimites[j][0]<<", ";
			}
			out<<std::endl;
			for(unsigned int j = 0; j < m_dim_anglelimites.size(); ++j)
			{
				out<<m_dim_anglelimites[j][1]<<", ";
			}
			out<<std::endl;
			out.close();
		}

		void setMinMaxLimits(int idx, double min, double max)
		{
			m_dim_anglelimites[idx](0) = min;
			m_dim_anglelimites[idx](1) = max;
			m_average_values[idx] = (min + max) * 0.5;
		}

		void updateAverageByMinMax(int idx)
		{
			m_average_values[idx] = (m_dim_anglelimites[idx](0) + m_dim_anglelimites[idx](1)) * 0.5;
		}

		std::vector<Joint*> m_joints;
		std::vector<Matrix3_, Eigen::aligned_allocator<Matrix3_> > m_joint_localRotations;
		std::vector<Matrix3_, Eigen::aligned_allocator<Matrix3_> > m_joint_globalOrientations;
		std::vector<Vector3_, Eigen::aligned_allocator<Vector3_> > m_joint_localTranslations;
		std::vector<Vector3_, Eigen::aligned_allocator<Vector3_> > m_joint_globalPositions;

		std::vector<Dim*> m_dims;
		std::vector<Matrix3_, Eigen::aligned_allocator<Matrix3_> > m_dim_localRotations;
		std::vector<Matrix3_, Eigen::aligned_allocator<Matrix3_> > m_dim_globalOrientations;
		std::vector<Vector3_, Eigen::aligned_allocator<Vector3_> > m_dim_localTranslations;
		std::vector<Vector3_, Eigen::aligned_allocator<Vector3_> > m_dim_globalPositions;

		std::vector<double> m_dim_values;
		std::vector<Vector3_, Eigen::aligned_allocator<Vector3_> > m_dim_axis;
		std::vector<Vector2_, Eigen::aligned_allocator<Vector2_> > m_dim_anglelimites;
		std::vector<Vector2_, Eigen::aligned_allocator<Vector2_> > m_dim_anglelimites_default;
		std::string m_name;

		std::vector<double> m_average_values;
		std::vector<double> m_posture_variation;

		virtual bool loadFromFile(const std::string& fileName);
		virtual void read(std::istream& in);

		void draw(int type);

		void setDimValueZero(){
			for(int i = 0; i < m_dim_values.size(); ++i){
				m_dim_values[i] = 0;
			}
		}

	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};

	struct IKTree : public IKChain
	{
	std::vector<int> m_joint_end_effector_index;
	std::vector<int> m_dim_end_effector_index;
	std::vector<int> m_joint_identifier;
	std::vector<int> m_dim_identifier;

	virtual bool loadFromFile(const std::string& fileName) override;
	virtual void read(std::istream& in) override;
	};
}


