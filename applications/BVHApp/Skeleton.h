#pragma once

#include <vector>
#include <iostream>
#include <Eigen/Dense>
#include <fstream>

	typedef Eigen::MatrixXd MatrixX_;
	typedef Eigen::Matrix3d Matrix3_;
	typedef Eigen::VectorXd VectorX_;
	typedef Eigen::Vector3d Vector3_;
	typedef Eigen::Vector4d Vector4_;
	typedef Eigen::Vector2d Vector2_;
	typedef Eigen::AngleAxisd AngleAxis_;


struct Skeleton
{
	enum DimType
	{
		Zposition,
		Yposition,
		Xposition,
		Zrotation,
		Yrotation,
		Xrotation,
	};

	struct Dim
	{
		int m_lastIdx;
		int m_idx;
		std::string m_name;
		DimType m_type;
	};

	struct Joint
	{
		std::string m_name;
		int m_index;
		int m_index_parent;
		int m_dof;
		int m_level;
		std::vector<int> m_dims;
	};

	Skeleton(const std::string& name=""): m_name(name)
	{
	}

	Joint* addJoint(int parent, int dof, Vector3_ offset, const std::string& name);

	void reset();
	void resetValues();

	void updateDimLocalRotation(int idx)
	{
		m_dim_localRotations[idx] = AngleAxis_(m_dim_values[idx], m_dim_axis[idx]);	
	}


	void updateDim(int idx)
	{
		updateDimLocalRotation(idx);
		Dim& dim = m_dims[idx];
		if(dim.m_lastIdx >= 0)
		{
			m_dim_globalPositions[dim.m_idx] = m_dim_globalPositions[dim.m_lastIdx] + m_dim_globalOrientations[dim.m_lastIdx] * m_dim_localTranslations[dim.m_idx];
			m_dim_globalOrientations[dim.m_idx] = m_dim_globalOrientations[dim.m_lastIdx] * m_dim_localRotations[dim.m_idx];	
		}
		else
		{
			m_dim_globalPositions[dim.m_idx] = m_dim_localTranslations[dim.m_idx] + Vector3_(m_dim_values[0], m_dim_values[1], m_dim_values[2]);
			m_dim_globalOrientations[dim.m_idx] = m_dim_localRotations[dim.m_idx];
		}
	}

	///*void updateAllDims()
	//{
	//	for(unsigned int i = 0; i < m_dims.size(); ++i)
	//	{
	//		updateDim(i);
	//	}
	//}*/

	void updateJoint(int idx)
	{
		Joint& current = m_joints[idx];
		Matrix3_ temp = Matrix3_::Identity();
		for(unsigned int i = 0; i < current.m_dims.size(); ++i)
		{
			Dim& dim = m_dims[current.m_dims[i]];
			updateDim(dim.m_idx);
			temp = temp * m_dim_localRotations[dim.m_idx];	
		}
		m_joint_localRotations[idx] = temp;
		int last = current.m_dof - 1;
		if(last > 0)
		{
			m_joint_globalOrientations[idx] = m_dim_globalOrientations[current.m_dims[last]];
			m_joint_globalPositions[idx] = m_dim_globalPositions[current.m_dims[last]];
		}else
		{
			m_joint_globalOrientations[idx] = m_joint_globalOrientations[current.m_index_parent];
			m_joint_globalPositions[idx] = m_joint_globalPositions[current.m_index_parent] + m_joint_globalOrientations[idx] * m_joint_offsets[idx];
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
		for(unsigned int j = 0; j < m_dim_values.size(); ++j)
		{
			out<<m_dim_values[j]<<", ";
		}
		out<<std::endl;
		out<<std::endl;
		out.close();
	}


	void buildJacobian(const std::vector<int>& endeffectors, MatrixX_& m_jacobian);

	std::vector<Joint> m_joints;
	std::vector<Vector3_, Eigen::aligned_allocator<Vector3_> > m_joint_offsets;
	std::vector<Matrix3_, Eigen::aligned_allocator<Matrix3_> > m_joint_localRotations;
	std::vector<Matrix3_, Eigen::aligned_allocator<Matrix3_> > m_joint_globalOrientations;
	std::vector<Vector3_, Eigen::aligned_allocator<Vector3_> > m_joint_localTranslations;
	std::vector<Vector3_, Eigen::aligned_allocator<Vector3_> > m_joint_globalPositions;

	std::vector<Dim> m_dims;
	std::vector<Matrix3_, Eigen::aligned_allocator<Matrix3_> > m_dim_localRotations;
	std::vector<Matrix3_, Eigen::aligned_allocator<Matrix3_> > m_dim_globalOrientations;
	std::vector<Vector3_, Eigen::aligned_allocator<Vector3_> > m_dim_localTranslations;
	std::vector<Vector3_, Eigen::aligned_allocator<Vector3_> > m_dim_globalPositions;
	std::vector<double> m_dim_values;
	std::vector<double> m_dim_last_values;
	std::vector<Vector3_, Eigen::aligned_allocator<Vector3_> > m_dim_axis;
	std::string m_name;

	std::vector<int> m_endeffectors;
	MatrixX_ m_jacobian;

	//virtual bool loadFromFile(const std::string& fileName);
	//virtual void read(std::istream& in);

	void draw(int type);

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};