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

	bool JacobianDLSSolver::solve(IKChain* chain, Vector3_ target, bool enableConstraints)
	{

		double beta = 0.0000005;

#if( defined( _DEBUG ) || defined( DEBUG ) )
		clock_t time = clock();
#endif
		chain->updateAllDims();
		int tries = 0;
		int columnDim = chain->m_dims.size();
		MatrixX_ jacobian(3, columnDim);
		Vector3_& endpos = chain->m_dim_globalPositions.back();
		Vector3_ distance = (target-endpos);

		std::vector<double> initValue = chain->m_dim_values;
		std::vector<bool> lock;
		for(unsigned int i = 0; i < initValue.size(); ++i)
		{
			lock.push_back(false);
		}

		VectorX_ v(chain->m_average_values.size());
		

		//double beta = 0.5f;

		while (++tries < m_maxTries &&
			distance.norm() > m_targetThreshold)
		{
			Vector3_ dT = distance;// * beta;

			for(unsigned int i = 0; i < chain->m_average_values.size();++i)
			{
				v(i) = chain->m_average_values[i] - chain->m_dim_values[i];
			}
			//v.normalize();

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
				if(!lock[j])
				{
					jacobian(0, j) = /*clamp(0 == axisXYZgradient(0)? 0.000001:*/ axisXYZgradient(0)/*, chain->m_dedr_min[j][0], chain->m_dedr_max[j][0])*/;// * m_stepweight;
					jacobian(1, j) =/* clamp(0 == axisXYZgradient(1)? 0.000001:*/ axisXYZgradient(1)/*, chain->m_dedr_min[j][1], chain->m_dedr_max[j][1])*/;// * m_stepweight;
					jacobian(2, j) =/* clamp(0 == axisXYZgradient(2)? 0.000001:*/ axisXYZgradient(2)/*, chain->m_dedr_min[j][2], chain->m_dedr_max[j][2])*/;// * m_stepweight;
				}else
				{
					jacobian(0, j) = 0;// * m_stepweight;
					jacobian(1, j) = 0;// * m_stepweight;
					jacobian(2, j) = 0;// * m_stepweight;
				}
			}

			MatrixX_ jacobianTranspose = jacobian.transpose();
#define USINGMYDLS
#ifdef USINGMYDLS
			
			MatrixX_ a_ =  jacobianTranspose * jacobian;
			MatrixX_ damp=MatrixX_::Identity(a_.rows(), a_.cols());
			MatrixX_ betax = MatrixX_::Identity(a_.rows(), a_.cols());
				for(int i = 0; i < a_.rows(); ++i)
				{
					damp(i,i) = m_dampling / (i * 100+1); 
					betax(i,i) = damp(i,i) /100.0;// beta;
				}
				MatrixX_ temp = damp + betax;
				MatrixX_ a = a_ + temp * MatrixX_::Identity(a_.rows(), a_.cols());
			MatrixX_ b = jacobianTranspose * dT + beta * v;
			VectorX_ dR = a.inverse() * b;
#else
			MatrixX_ a =  jacobian * jacobianTranspose;

			MatrixX_ dls = jacobianTranspose * ( a +  m_dampling * MatrixX_::Identity(a.rows(), a.cols())).inverse();
			VectorX_ dR = dls * dT;
#endif

			for(int i = 0; i < columnDim; ++i)
			{
				double temp = chain->m_dim_values[i] + dR[i];
				double value = clamp(temp, chain->m_dim_anglelimites[i][0], chain->m_dim_anglelimites[i][1]);
				if((value - chain->m_dim_values[i] - dR[i]) > 0.0000001)
				{
					lock[i] = true;
					//chain->m_posture_variation(i) = 0;
				}
				chain->m_dim_values[i] = value;
				chain->m_dim_values[i] = castPiRange(chain->m_dim_values[i]);
				//chain->m_dim_localRotations[i] = AngleAxis_(chain->m_dim_values[i], chain->m_dim_axis[i]);	
			}
			chain->updateAllDims();
			endpos = chain->m_dim_globalPositions.back();
			distance = (target - endpos);
		}
#if( defined( _DEBUG ) || defined( DEBUG ) )
		time = clock() - time;
		int ms = double(time) / CLOCKS_PER_SEC * 1000;
		std::cout<<"timee elapsed: "<<ms<<std::endl;
		std::cout<<"iterations: "<<tries<< "distance: "<<distance.norm()<<std::endl;
#endif
		chain->update();
		if (tries == m_maxTries)
		{
			return false;
		}

		return true;
	}

	bool JacobianDLSSolver::solve(IKChain* chain, Vector3_ target, Vector3_ orientation, bool enableConstraints)
	{
#if( defined( _DEBUG ) || defined( DEBUG ) )
		clock_t time = clock();
#endif
		int tries = 0;
		int columnDim = chain->m_dims.size();
		MatrixX_ jacobian(3, columnDim);
		chain->update();
		Vector3_& endpos = chain->m_dim_globalPositions.back();
		Vector3_ distance = (target-endpos);

		double beta = 0.5f;

		while (++tries < m_maxTries &&
			distance.norm() > m_targetThreshold)
		{
			Vector3_ dT = distance * beta;

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
			MatrixX_ dls = jacobianTranspose * ( a +  m_dampling * MatrixX_::Identity(a.rows(), a.cols())).inverse();
			VectorX_ dR = dls * dT;

			for(int i = 0; i < columnDim; ++i)
			{
				chain->m_dim_values[i] = castPiRange(chain->m_dim_values[i] + dR[i]);
				chain->m_dim_values[i] = clamp(chain->m_dim_values[i], chain->m_dim_anglelimites[i][0], chain->m_dim_anglelimites[i][1]);
				chain->m_dim_localRotations[i] = AngleAxis_(chain->m_dim_values[i], chain->m_dim_axis[i]);	
			}
			chain->update();
			endpos = chain->m_dim_globalPositions.back();
			distance = (target - endpos);
		}
#if( defined( _DEBUG ) || defined( DEBUG ) )
		time = clock() - time;
		int ms = double(time) / CLOCKS_PER_SEC * 1000;
		std::cout<<"timee elapsed: "<<ms<<std::endl;
		std::cout<<"iterations: "<<tries<< "distance: "<<distance.norm()<<std::endl;
#endif
		if (tries == m_maxTries)
		{
			return false;
		}

		return true;
	}
}
