/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianDLSSolver.cpp
* @brief 
* @date 1/2/2011
*/

#include "JacobianDLSSolver.h"
#include <ctime>
namespace Etoile
{

	void JacobianDLSSolver::solveOneStep(IKChain* chain, Vector3_ target, bool enableConstraints)
	{


		chain->updateAllDims();
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
		
				jacobian(0, j) = axisXYZgradient(0);
				jacobian(1, j) = axisXYZgradient(1);
				jacobian(2, j) = axisXYZgradient(2);
				
			}

			MatrixX_ jacobianTranspose = jacobian.transpose();
#define USINGMYDLS
#ifdef USINGMYDLS

			MatrixX_ jtj =  jacobianTranspose * jacobian;
			MatrixX_ lamdaI = MatrixX_::Identity(jtj.rows(), jtj.cols());
			MatrixX_ betax = MatrixX_::Zero(jtj.rows(), jtj.cols());
			for(int i = 0; i < jtj.rows(); ++i)
			{
				lamdaI(i,i) = chain->m_posture_variation[i];   //m_dampling / ( pow(10.0, i/3) * +1); 
				//assert(lamdaI(i,i) > 0.0000000000000000000001 && lamdaI(i,i) < 99999999999999);
			}

			MatrixX_ a = jtj + lamdaI;
			MatrixX_ b = jacobianTranspose * distance;// + thetax * v2;
			VectorX_ dR = a.inverse() * b;
#else
			MatrixX_ a =  jacobian * jacobianTranspose;

			MatrixX_ dls = jacobianTranspose * ( a +  m_dampling * MatrixX_::Identity(a.rows(), a.cols())).inverse();
			VectorX_ dR = dls * dT;
#endif

			for(int i = 0; i < columnDim; ++i)
			{
				chain->m_dim_values[i] = castPiRange(chain->m_dim_values[i] + dR[i]);
				chain->m_dim_values[i] = clamp(chain->m_dim_values[i], chain->m_dim_anglelimites[i][0], chain->m_dim_anglelimites[i][1]);
			}
			//std::cout<<chain->m_dim_values[0]<<std::endl;
			chain->updateAllDims();
	}

}