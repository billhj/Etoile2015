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
	void JacobianTransposeSolver::solveOneStep(IKChain* chain, Vector3_ target, bool enableConstraints)
	{
		int tries = 0;
		int columnDim = chain->m_dims.size();
		MatrixX_ jacobian(3, columnDim);
		Vector3_& endpos = chain->m_dim_globalPositions.back();
		Vector3_ distance = (target-endpos) * 0.5;


		for(unsigned int j = 0; j < chain->m_dims.size(); ++j)
		{
			IKChain::Dim* dim = chain->m_dims[j];
			Vector3_& jointPos = chain->m_dim_globalPositions[dim->m_idx];
			Vector3_ boneVector = endpos - jointPos;
			Vector3_ axis = chain->m_dim_axis[dim->m_idx];
			int lastDim = dim->m_lastIdx;
			if(lastDim >= 0)
			{
				axis = chain->m_dim_globalOrientations[lastDim] * axis;
			}
			Vector3_ axisXYZgradient = axis.cross(boneVector);
			jacobian(0, dim->m_idx) = axisXYZgradient(0);// * m_stepweight;
			jacobian(1, dim->m_idx) = axisXYZgradient(1);// * m_stepweight;
			jacobian(2, dim->m_idx) = axisXYZgradient(2);// * m_stepweight;
		}

#if( defined( _DEBUG ) || defined( DEBUG ) )
		//std::cout<<"jacobian: "<<jacobian<<std::endl;
#endif
		MatrixX_ jacobianTranspose = jacobian.transpose();
		VectorX_ dR = jacobianTranspose * distance;

		for(int i = 0; i < columnDim; ++i)
		{
			chain->m_dim_values[i] = castPiRange(chain->m_dim_values[i] + dR[i]);
			chain->m_dim_values[i] = clamp(chain->m_dim_values[i], chain->m_dim_anglelimites[i][0], chain->m_dim_anglelimites[i][1]);
		}

		chain->updateAllDims();
	}


}
