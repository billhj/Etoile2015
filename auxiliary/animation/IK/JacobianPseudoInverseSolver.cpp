/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianPseudoInverseSolver.cpp
* @brief 
* @date 1/2/2011
*/

#include "JacobianPseudoInverseSolver.h"
#include <ctime>
namespace Etoile
{
	void JacobianPseudoInverseSolver::solveOneStep(IKChain* chain, Vector3_ target, bool enableConstraints)
	{
#if( defined( _DEBUG ) || defined( DEBUG ) )
		clock_t time = clock();
#endif

		int tries = 0;
		int columnDim = chain->m_dims.size();
		MatrixX_ jacobian(3, columnDim);
		Vector3_& endpos = chain->m_dim_globalPositions.back();
		Vector3_ distance = (target-endpos);

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
			jacobian(0, dim->m_idx) = 0 == axisXYZgradient(0)? 0.000001: axisXYZgradient(0);// * m_stepweight;
			jacobian(1, dim->m_idx) = 0 == axisXYZgradient(1)? 0.000001: axisXYZgradient(1);// * m_stepweight;
			jacobian(2, dim->m_idx) = 0 == axisXYZgradient(2)? 0.000001: axisXYZgradient(2);// * m_stepweight;
		}


		MatrixX_ jacobianTranspose = jacobian.transpose();
		MatrixX_ a =  jacobian * jacobianTranspose;
		MatrixX_ aInv = a.inverse();
		MatrixX_ pseudoInverse = jacobianTranspose * aInv;


		VectorX_ dR = pseudoInverse * distance;

		for(int i = 0; i < columnDim; ++i)
		{
			chain->m_dim_values[i] = castPiRange(chain->m_dim_values[i] + dR[i]);
			chain->m_dim_values[i] = clamp(chain->m_dim_values[i], chain->m_dim_anglelimites[i][0], chain->m_dim_anglelimites[i][1]);
		}

		chain->updateAllDims();
	
	}


	//	MatrixX_ JacobianPseudoInverseSolver::computeRotations(Eigen::MatrixX_ jacobian, Vector3_ dT)
	//	{
	//		
	////#if( defined( _DEBUG ) || defined( DEBUG ) )
	////		std::cout<<"a: "<<std::endl<<a<<std::endl;
	////		std::cout<<"aInv: "<<std::endl<<aInv<<std::endl;
	////			std::cout<<"pseudoInverse: "<<std::endl<<pseudoInverse<<std::endl;
	////#endif
	//		return pseudoInverse * dT;
	//	}
}
