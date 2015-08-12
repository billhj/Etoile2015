/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianRcompensedDLSSolver.cpp
* @brief 
* @date 1/2/2011
*/

#include "JacobianRcompensedDLSSolver.h"
#include <ctime>
//#include "FLog.h" 
namespace Etoile
{

	void JacobianRcompensedDLSSolver::solveOneStep(IKChain* chain, Vector3_ target, bool enableConstraints)
	{
		int columnDim = chain->m_dims.size();
		Vector3_& endpos = chain->m_dim_globalPositions.back();
		Vector3_ distance = (target-endpos);

		MatrixX_ jacobian(3, columnDim);

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

		MatrixX_ jtj =  jacobianTranspose * jacobian;
		MatrixX_ lamdaI = MatrixX_::Identity(jtj.rows(), jtj.cols());

		for(int i = 0; i < jtj.rows(); ++i)
		{
			lamdaI(i,i) = chain->m_posture_variation[i];   
		}
		MatrixX_ a = jtj + lamdaI;
		MatrixX_ b = jacobianTranspose * distance; 

		if(recompense)
		{
			MatrixX_ betaI = MatrixX_::Zero(jtj.rows(), jtj.cols());
			VectorX_ beta = a * recompenseValue;
			VectorX_ tr = (v0 - recompenseValue);
			assert((v0 - recompenseValue).norm() > 0.00000001);

			for(int i = 0; i < jtj.rows(); ++i)
			{
				if(beta(i) != 0)
					betaI(i,i) = beta(i) / tr(i);  
			}
			//std::cout<<betaI<<std::endl;
			a = a + betaI;
		}

		VectorX_ dR = a.inverse() * b;

		for(int i = 0; i < columnDim; ++i)
		{
			chain->m_dim_values[i] = chain->m_dim_values[i] + dR[i];
			chain->m_dim_values[i] = castPiRange(chain->m_dim_values[i]);
			
			double value = clamp(chain->m_dim_values[i], chain->m_dim_anglelimites[i][0], chain->m_dim_anglelimites[i][1]);
			if(abs(value - chain->m_dim_values[i]) > 0.0000001)
			{
				recompenseValue(i) = value - chain->m_dim_values[i];
				recompense = true;
			}else
			{
				recompenseValue(i) = 0;
			}
			v0(i) = chain->m_average_values[i] - chain->m_dim_values[i];
			chain->m_dim_values[i] = value;
		}
		//v0 *= 15;
		chain->updateAllDims();
	}

	bool JacobianRcompensedDLSSolver::solve(IKChain* chain, Vector3_ target, bool enableConstraints)
	{

#if( defined( _DEBUG ) || defined( DEBUG ) )
		clock_t time = clock();
#endif
		chain->updateAllDims();
		Vector3_& endpos = chain->m_dim_globalPositions.back();
		Vector3_ distance = (target-endpos);
		v0 = VectorX_::Zero(chain->m_average_values.size());
		recompenseValue = VectorX_::Zero(chain->m_average_values.size());
		recompense = false;
		int tries = 0;
		while (tries < m_maxTries &&
			(distance.norm() > m_targetThreshold) || tries == 0 )
		{
			++tries;
			solveOneStep(chain, target, enableConstraints);
			endpos = chain->m_dim_globalPositions.back();
			distance = (target - endpos);
		}

#if( defined( _DEBUG ) || defined( DEBUG ) )
		time = clock() - time;
		int ms = double(time) / CLOCKS_PER_SEC * 1000;
		std::cout<<"timee elapsed: "<<ms<<std::endl;
		std::cout<<"iterations: "<<tries<< " distance: "<<distance.norm()<<std::endl;
#endif

		chain->update();
		if (distance.norm() > m_targetThreshold)
		{
			return false;
		}

		return true;
	}


}
