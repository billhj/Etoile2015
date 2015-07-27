/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianDLSSVDSolver.cpp
* @brief 
* @date 1/2/2011
*/

#include "JacobianDLSSVDSolver.h"
#include <ctime>
#include "flog.h"

namespace Etoile
{
	double epsilon = 1.2;
#define HARDCONSTRAINTS
#ifdef HARDCONSTRAINTS
	bool JacobianDLSSVDSolver::solve(IKChain* chain, Vector3_ target, bool enableConstraints)
	{
		//#if( defined( _DEBUG ) || defined( DEBUG ) )
		//		clock_t time = clock();
		//#endif
		chain->updateAllDims();
		int tries = 0;
		std::vector<double> initValue = chain->m_dim_values;
		int columnDim = chain->m_dims.size();
		MatrixX_ jacobian(3, columnDim);
		Vector3_& endpos = chain->m_dim_globalPositions.back();
		Vector3_ distance = (target-endpos);

		//double beta = 0.5f;

		while (++tries < m_maxTries &&
			distance.norm() > m_targetThreshold)
		{
			Vector3_ dT = distance;// * beta;

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
				jacobian(0, dim->m_idx) = /*0 == axisXYZgradient(0)? 0.000001:*/ axisXYZgradient(0);// * m_stepweight;
				jacobian(1, dim->m_idx) = /*0 == axisXYZgradient(1)? 0.000001:*/ axisXYZgradient(1);// * m_stepweight;
				jacobian(2, dim->m_idx) = /*0 == axisXYZgradient(2)? 0.000001:*/ axisXYZgradient(2);// * m_stepweight;
			}


			Eigen::JacobiSVD<MatrixX_> svd(jacobian,  Eigen::ComputeFullU| Eigen::ComputeFullV);
			VectorX_ s =  svd.singularValues();
			MatrixX_ u =  svd.matrixU();
			MatrixX_ v =  svd.matrixV();

			double lamda2 = 0;
			double minvalue = 1000;
			for (int i = 0; i < s.size(); ++i) {
				double value = s(i);
				if (value < minvalue) {
					minvalue = value;
				}
			}
			if (minvalue >= epsilon) {
				lamda2 = 0;
			} else {
				lamda2 = (1 - (minvalue / epsilon)) * m_dampling_max;
			}

			MatrixX_ e(u.cols(),v.rows()); e.setZero();
			for (int i = 0; i < s.size(); ++i) {
				double value = s(i);
				e(i, i) = value / (value * value + lamda2);
			}

			MatrixX_ dls = v * e.transpose() * u.transpose();
			VectorX_ dR = dls * dT;

			for(int i = 0; i < columnDim; ++i)
			{
				chain->m_dim_values[i] = chain->m_dim_values[i] + dR[i];
				chain->m_dim_values[i] = castPiRange(chain->m_dim_values[i]);
				chain->m_dim_values[i] = clamp(chain->m_dim_values[i], chain->m_dim_anglelimites[i][0], chain->m_dim_anglelimites[i][1]);//, chain->m_average_values[i]);
				if(enableConstraints)
					chain->m_dim_values[i] = clampDr(chain->m_dim_values[i], initValue[i], chain->m_drLimits_positive[i], chain->m_drLimits_negative[i]);
				chain->m_dim_localRotations[i] = AngleAxis_(chain->m_dim_values[i], chain->m_dim_axis[i]);
			}

			chain->updateAllDims();
			endpos = chain->m_dim_globalPositions.back();
			distance = (target - endpos);
		}
		//#if( defined( _DEBUG ) || defined( DEBUG ) )
		//		time = clock() - time;
		//		int ms = double(time) / CLOCKS_PER_SEC * 1000;
		//		std::cout<<"timee elapsed: "<<ms<<std::endl;
		std::cout<<"iterations: "<<tries<< "distance: "<<distance.norm()<<std::endl;
		//#endif

		chain->update();
		if (tries == m_maxTries)
		{
			return false;
		}
		return true;
	}


#else

	bool JacobianDLSSVDSolver::solve(IKChain* chain, Vector3_ target, bool enableConstraints)
	{
#if( defined( _DEBUG ) || defined( DEBUG ) )
		clock_t time = clock();
#endif
		int tries = 0;
		int columnDim = chain->m_dims.size();

		for(int i = 0; i < columnDim; ++i)
		{
			chain->m_dim_localRotations[i] = AngleAxis_(chain->m_dim_values[i], chain->m_dim_axis[i]);	
		}

		std::vector<double> initValue = chain->m_dim_values;
		//double dtThreasure = 0.03;

		MatrixX_ jacobian(3, columnDim);
		chain->update();
		Vector3_& endpos = chain->m_dim_globalPositions.back();
		Vector3_ distance = (target-endpos);

		//double beta = 0.5f;

		while (++tries < m_maxTries &&
			distance.norm() > m_targetThreshold)
		{
			Vector3_ dT = distance;// * beta;

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
				jacobian(0, dim->m_idx) = /*0 == axisXYZgradient(0)? 0.000001:*/ axisXYZgradient(0);// * m_stepweight;
				jacobian(1, dim->m_idx) = /*0 == axisXYZgradient(1)? 0.000001:*/ axisXYZgradient(1);// * m_stepweight;
				jacobian(2, dim->m_idx) = /*0 == axisXYZgradient(2)? 0.000001:*/ axisXYZgradient(2);// * m_stepweight;
			}


			Eigen::JacobiSVD<MatrixX_> svd(jacobian,  Eigen::ComputeFullU| Eigen::ComputeFullV);
			VectorX_ s =  svd.singularValues();
			MatrixX_ u =  svd.matrixU();
			MatrixX_ v =  svd.matrixV();

			double lamda2 = 0;
			double minvalue = 1000;
			for (int i = 0; i < s.size(); ++i) {
				double value = s(i);
				if (value < minvalue) {
					minvalue = value;
				}
			}
			if (minvalue >= epsilon) {
				lamda2 = 0;
			} else {
				lamda2 = (1 - (minvalue / epsilon)) * m_dampling_max;
			}

			MatrixX_ e(u.cols(),v.rows()); e.setZero();
			for (int i = 0; i < s.size(); ++i) {
				double value = s(i);
				e(i, i) = value / (value * value + lamda2);
			}

			MatrixX_ dls = v * e.transpose() * u.transpose();
			VectorX_ dR = dls * dT;

			for(int i = 0; i < columnDim; ++i)
			{
				chain->m_dim_values[i] = chain->m_dim_values[i] + dR[i];
				chain->m_dim_values[i] = castPiRange(chain->m_dim_values[i]);
				chain->m_dim_values[i] = clamp(chain->m_dim_values[i], chain->m_dim_anglelimites[i][0], chain->m_dim_anglelimites[i][1]);//, chain->m_average_values[i]);
				if(enableConstraints)
					chain->m_dim_values[i] = clampDr(chain->m_dim_values[i], initValue[i], chain->m_drLimits_positive[i], chain->m_drLimits_negative[i]);


				/*if(!enableConstraints)
				{
				std::cout<< chain->m_dim_values[i]<<" ";
				}*/
				chain->m_dim_localRotations[i] = AngleAxis_(chain->m_dim_values[i], chain->m_dim_axis[i]);	
			}
			/*if(!enableConstraints)
			{
			std::cout<< std::endl;
			}*/

			chain->update();
			endpos = chain->m_dim_globalPositions.back();
			distance = (target - endpos);
		}
#if( defined( _DEBUG ) || defined( DEBUG ) )
		time = clock() - time;
		int ms = double(time) / CLOCKS_PER_SEC * 1000;
		FLOG<<"timee elapsed: "<<ms<<std::endl;
		FLOG<<"iterations: "<<tries<< "distance: "<<distance.norm()<<std::endl;
#endif
		if (tries == m_maxTries)
		{
			return false;
		}
		return true;
	}

#endif

	double JacobianDLSSVDSolver::constraintSolved(double v, double minV, double maxV, double averageV)
	{	
		double value;
		if (value > maxV || value < minV) {
			value += (averageV - value) * 0.5;
		}

		return value;
	}

	double JacobianDLSSVDSolver::clampDr(double value, double original, double limitP, double limitN)
	{
		double dr = value - original;
		if(dr > limitP)
		{
			dr = limitP;
		}else if(dr < limitN)
		{
			dr = limitN;
		}

		return original + dr;
	}

}


