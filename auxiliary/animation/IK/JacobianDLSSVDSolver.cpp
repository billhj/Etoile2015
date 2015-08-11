/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianDLSSVDSolver.cpp
* @brief 
* @date 1/2/2011
*/

#include "JacobianDLSSVDSolver.h"


namespace Etoile
{

	double epsilon = 1.2;
	void JacobianDLSSVDSolver::solveOneStep(IKChain* chain, Vector3_ target, bool enableConstraints)
	{
		chain->updateAllDims();
		int tries = 0;
		int columnDim = chain->m_dims.size();
		MatrixX_ jacobian(3, columnDim);
		Vector3_& endpos = chain->m_dim_globalPositions.back();
		Vector3_ distance = (target-endpos);

		MatrixX_ p_i = MatrixX_::Identity(columnDim,columnDim);


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



		VectorX_ dR = dls * distance;// + p * chain->m_posture_variation;


		for(int i = 0; i < columnDim; ++i)
		{
			chain->m_dim_values[i] = castPiRange(chain->m_dim_values[i] + dR[i]);
			chain->m_dim_values[i] = clamp(chain->m_dim_values[i], chain->m_dim_anglelimites[i][0], chain->m_dim_anglelimites[i][1]);
		}

		chain->updateAllDims();

	}


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


