#include "Skeleton.h"


Skeleton::Joint* Skeleton::addJoint(int parent, int dof, const std::string& name)
{
	int size = m_joints.size();
	m_joints.push_back(Joint());
	Joint& joint = m_joints[size];
	joint.m_index = size;

	joint.m_index_parent = parent;
	joint.m_name = name;
	joint.m_dof = dof;

	joint.m_dims.resize(joint.m_dof);
	for(int i = 0; i < joint.m_dof; ++i)
	{
		int currentDimIndex = this->m_dims.size();
		joint.m_dims[i] = currentDimIndex;
		this->m_dims.push_back(Dim());
		Dim& currentDim = this->m_dims[currentDimIndex];
		if(i == 0)
		{
			if( parent >= 0)
			{
				currentDim.m_lastIdx = this->m_joints[parent].m_dims.back();
			}
			else
			{
				currentDim.m_lastIdx = -1;
			}
		}else
		{
			currentDim.m_lastIdx = joint.m_dims[i - 1];
		}

		this->m_dim_axis.push_back(Vector3_::Zero());

		this->m_dim_localRotations.push_back(Matrix3_::Identity());
		this->m_dim_globalOrientations.push_back(Matrix3_::Identity());
		this->m_dim_localTranslations.push_back(Vector3_::Zero());
		this->m_dim_globalPositions.push_back(Vector3_::Zero());
		this->m_dim_values.push_back(0);
	}

	this->m_joint_localRotations.push_back(Matrix3_::Identity());
	this->m_joint_globalOrientations.push_back(Matrix3_::Identity());
	this->m_joint_offsets.push_back(Vector3_::Zero());
	this->m_joint_localTranslations.push_back(Vector3_::Zero());
	this->m_joint_globalPositions.push_back(Vector3_::Zero());
	return &joint;
}

void Skeleton::reset()
{
	for(int i = 0; i < m_dims.size();++i)
	{
		m_dim_localRotations[i].setIdentity();
		m_dim_globalOrientations[i].setIdentity();
		m_dim_values[i] = 0;
	}
	update();
}

void Skeleton::resetValues()
{
	for(int i = 0; i < m_dims.size();++i)
	{
		m_dim_values[i] = 0;
	}
	updateAllDims();
	update();
}