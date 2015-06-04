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
	using namespace Eigen;
	bool JacobianTransposeSolver::compute(IKChain* chain, Vector3f target, bool enableConstraints)
	{
		int tries = 0;
		int columnDim = chain->m_localRotations.size();
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
				IKChain::Joint* joint = chain->m_joints[i];
				std::vector<IKChain::Dim>& dims = joint->m_dims;
				for(unsigned int j = 0; j < dims.size(); ++j)
				{
					IKChain::Dim& dim = dims[j];
					Vector3f& jointPos = chain->m_globalPositions[dim.m_idx];
					Vector3f boneVector = endpos - jointPos;
					Vector3f axis = dim.m_axis;
					int lastDim = dim.m_lastIdx;
					if(lastDim >= 0)
					{
						axis = chain->m_globalOrientations[lastDim] * axis;
					}
					Vector3f axisXYZgradient = axis.cross(boneVector);
					jacobian(0, dim.m_idx) = axisXYZgradient(0);// * m_stepweight;
					jacobian(1, dim.m_idx) = axisXYZgradient(1);// * m_stepweight;
					jacobian(2, dim.m_idx) = axisXYZgradient(2);// * m_stepweight;
				}
			}
#if( defined( _DEBUG ) || defined( DEBUG ) )
			std::cout<<"jacobian: "<<jacobian<<std::endl;
#endif
			MatrixXf jacobianTranspose = jacobian.transpose();
			MatrixXf dR = jacobianTranspose * dT;

			int m = 0;
			for(unsigned int i = 0; i < chain->m_joints.size(); ++i)
			{
				IKChain::Joint* joint = chain->m_joints[i];

				std::vector<IKChain::Dim>& dims = joint->m_dims;
				for(unsigned int j = 0; j < dims.size(); ++j, ++m)
				{
					dims[j].m_value = castPiRange(dims[j].m_value + dR(m));
					chain->m_localRotations[dims[j].m_idx] = AngleAxisf(dims[j].m_value, dims[j].m_axis);;
				}
				chain->updateJoint(i);
			}

			endpos = chain->m_globalPositions.back();
			distance = (target - endpos);
#if( defined( _DEBUG ) || defined( DEBUG ) )
			std::cout<<"endpos: "<<endpos.transpose()<<"     distance:  " << distance.norm()<<std::endl;
#endif
		}

		if (tries == m_maxTries)
		{
			return false;
		}
#if( defined( _DEBUG ) || defined( DEBUG ) )
		std::cout<<"iterations: "<<tries<<std::endl;
#endif
		return true;
	}

	
}
