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
	bool JacobianTransposeSolver::solve(Vector3_ target, bool enableConstraints)
	{
		int tries = 0;
		int columnDim = p_chain->m_localRotations.size();
		MatrixX_ jacobian(3, columnDim);
		p_chain->update();
		Vector3_& endpos = p_chain->m_globalPositions.back();
		Vector3_ distance = (target-endpos);

		double beta = 0.1f;

		while (++tries < m_maxTries &&
			distance.norm() > m_targetThreshold)
		{
			Vector3_ dT = distance * beta;
			for(unsigned int i = 0; i < p_chain->m_joints.size(); ++i)
			{
				IKChain::Joint* joint = p_chain->m_joints[i];
				std::vector<IKChain::Dim>& dims = joint->m_dims;
				for(unsigned int j = 0; j < dims.size(); ++j)
				{
					IKChain::Dim& dim = dims[j];
					Vector3_& jointPos = p_chain->m_globalPositions[dim.m_idx];
					Vector3_ boneVector = endpos - jointPos;
					Vector3_ axis = p_chain->m_axis[dim.m_idx];
					int lastDim = dim.m_lastIdx;
					if(lastDim >= 0)
					{
						axis = p_chain->m_globalOrientations[lastDim] * axis;
					}
					Vector3_ axisXYZgradient = axis.cross(boneVector);
					jacobian(0, dim.m_idx) = 0 == axisXYZgradient(0)? 0.000001: axisXYZgradient(0);// * m_stepweight;
					jacobian(1, dim.m_idx) = 0 == axisXYZgradient(1)? 0.000001: axisXYZgradient(1);// * m_stepweight;
					jacobian(2, dim.m_idx) = 0 == axisXYZgradient(2)? 0.000001: axisXYZgradient(2);// * m_stepweight;
				}
			}
#if( defined( _DEBUG ) || defined( DEBUG ) )
			//std::cout<<"jacobian: "<<jacobian<<std::endl;
#endif
			MatrixX_ jacobianTranspose = jacobian.transpose();
			VectorX_ dR = jacobianTranspose * dT;

			for(int i = 0; i < columnDim; ++i)
			{
				p_chain->m_values[i] = castPiRange(p_chain->m_values[i] + dR(i));
				p_chain->m_localRotations[i] = AngleAxis_(p_chain->m_values[i], p_chain->m_axis[i]);	
			}

			p_chain->update();
			endpos = p_chain->m_globalPositions.back();
			distance = (target - endpos);
#if( defined( _DEBUG ) || defined( DEBUG ) )
			//std::cout<<"endpos: "<<endpos.transpose()<<"     distance:  " << distance.norm()<<std::endl;
#endif
		}

		if (tries == m_maxTries)
		{
			return false;
		}
#if( defined( _DEBUG ) || defined( DEBUG ) )
		std::cout<<"iterations: "<<tries<< "distance: "<<distance.norm()<<std::endl;
#endif
		return true;
	}

	
}
