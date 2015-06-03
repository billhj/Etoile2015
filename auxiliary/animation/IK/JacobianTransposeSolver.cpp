/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianTransposeSolver.cpp
* @brief 
* @date 1/2/2011
*/

#include "JacobianTransposeSolver.h"
#include "math/MathHead.h"

namespace Etoile
{
	bool JacobianTransposeSolver::compute(IKChain* chain, Eigen::Vector3f target, bool enableConstraints)
	{
		int tries = 0;
		float distance;

		while (++tries < m_maxTries &&
			distance > m_targetThreshold)
		{
			
		}
		
		if (tries == m_maxTries)
		{
			return false;
		}

		return true;
	}
}
