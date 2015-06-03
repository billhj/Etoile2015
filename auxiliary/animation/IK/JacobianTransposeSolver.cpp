/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianTransposeSolver.cpp
* @brief 
* @date 1/2/2011
*/

#include "JacobianTransposeSolver.h"

namespace Etoile
{
	float castPiRange(float value)
	{
		while(value > 3.14159265)
		{
			value -= 3.14159265;
		}
		while(value < -3.14159265)
		{
			value += 3.14159265;
		}
		return value;
	}

	using namespace Eigen;
	bool JacobianTransposeSolver::compute(IKChain* chain, Vector3f target, bool enableConstraints)
	{
		int tries = 0;
		int columnDim = chain->m_joints.size() * 3;
		MatrixXf jacobian(3, columnDim);

		chain->update();
		Vector3f& endpos = chain->m_globalPositions.back();
		Vector3f distance = (target-endpos);

		float beta = 0.1f;

		while (++tries < m_maxTries &&
			distance.norm() > m_targetThreshold)
		{
			Vector3f dT = distance * beta;
			for(unsigned int i = 0; i <  chain->m_joints.size(); ++i)
			{
				Vector3f& jointPos = chain->m_globalPositions[i];
				Vector3f boneVector = endpos - jointPos;
				IKChain::Joint* joint = chain->m_joints[i];
				Vector3f axis0 = joint->m_axis[0], axis1 = joint->m_axis[1], axis2 = joint->m_axis[2];
				int parentIndex = joint->m_index_parent;
				if(parentIndex >= 0)
				{
					axis0 = chain->m_globalOrientations[parentIndex] * axis0;
					axis1 = chain->m_globalOrientations[parentIndex] * axis1;
					axis2 = chain->m_globalOrientations[parentIndex] * axis2;
				}

				Vector3f axis0XYZ = axis0.cross(boneVector);
				int column = i * 3;
				jacobian(0, column) = axis0XYZ(0);
				jacobian(1, column) = axis0XYZ(1);
				jacobian(2, column) = axis0XYZ(2);

				Vector3f axis1XYZ = axis1.cross(boneVector);
				++column;
				jacobian(0, column) = axis1XYZ(0);
				jacobian(1, column) = axis1XYZ(1);
				jacobian(2, column) = axis1XYZ(2);

				Vector3f axis2XYZ = axis2.cross(boneVector);
				++column;
				jacobian(0, column) = axis2XYZ(0);
				jacobian(1, column) = axis2XYZ(1);
				jacobian(2, column) = axis2XYZ(2);
			}

			MatrixXf jacobianTranspose = jacobian.transpose();
			MatrixXf dR = jacobianTranspose * dT;

			int j = 0;
			for(unsigned int i = 0; i < chain->m_joints.size(); ++i)
			{
				IKChain::Joint* joint = chain->m_joints[i];
				joint->m_values[0] = castPiRange(joint->m_values[0] + dR(j));
				++j;
				joint->m_values[1] = castPiRange(joint->m_values[1] + dR(j));
				++j;
				joint->m_values[2] = castPiRange(joint->m_values[2] + dR(j));
				++j;

				chain->m_localRotations[i] = ;
				chain->updateJoint(i);
			}

			endpos = chain->m_globalPositions.back();
			distance = (target - endpos);
		}

		if (tries == m_maxTries)
		{
			return false;
		}

		return true;
	}

	
}
