/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianDLSSolver.cpp
* @brief 
* @date 1/2/2011
*/

#include "JacobianDLSSolver.h"
#include <ctime>
#include "FLog.h" 
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
		//std::vector<double> lock_value;
		for(unsigned int i = 0; i < initValue.size(); ++i)
		{
			lock.push_back(false);
			//lock_value.push_back(0);
		}

		VectorX_ v0(chain->m_average_values.size());
		VectorX_ recompense(chain->m_average_values.size());
		/*VectorX_ v1(chain->m_average_values.size());
		VectorX_ v2(chain->m_average_values.size());*/

		VectorX_ dr(chain->m_average_values.size());
		//double beta = 0.5f;
		if(enableConstraints == false)
		{
			chain->m_dim_last_values = chain->m_dim_values;
		}

		while (tries < m_maxTries &&
			distance.norm() > m_targetThreshold || tries == 0 )
		{
			++tries;
			Vector3_ dT = distance;// * beta;

			for(unsigned int i = 0; i < chain->m_average_values.size();++i)
			{
				v0(i) = chain->m_average_values[i] - chain->m_dim_values[i];
				//v1(i) = initValue[i] - chain->m_dim_last_values[i] - (chain->m_dim_values[i] - initValue[i]);//chain->m_dim_values[i] - initValue[i];
				//v2(i) = -chain->m_dim_values[i];
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
				jacobian(0, j) = axisXYZgradient(0);
				jacobian(1, j) = axisXYZgradient(1);
				jacobian(2, j) = axisXYZgradient(2);
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

			MatrixX_ jtj =  jacobianTranspose * jacobian;
			MatrixX_ lamdaI =MatrixX_::Identity(jtj.rows(), jtj.cols());
			MatrixX_ betax = MatrixX_::Zero(jtj.rows(), jtj.cols());
			for(int i = 0; i < jtj.rows(); ++i)
			{
				lamdaI(i,i) = m_lamda[i];   //m_dampling / ( pow(10.0, i/3) * +1); 
			}

			MatrixX_ temp = lamdaI + betax;// + gamax;
			MatrixX_ a = jtj + temp;
			MatrixX_ b = jacobianTranspose * dT + betax * v0;// + thetax * v2;
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
				if((value - temp) > 0.0000001)
				{
					lock[i] = true;
					recompense(i) = value - temp;
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
		FLOG<<"iterations: "<<tries<< " distance: "<<distance.norm()<<std::endl;
		for(int i = 0; i < m_lamda.size(); ++i)
		{
			FLOG<< m_lamda[i]<<" ";
		}
		FLOG<<std::endl;
		chain->update();
		chain->m_dim_last_values = initValue;
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
