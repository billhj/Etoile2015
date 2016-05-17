/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GPIKsolver.cpp
* @brief 
* @date 1/2/2011
*/
#include "GPIKsolver.h"


GPIKsolver::GPIKsolver(void)
{
}


GPIKsolver::~GPIKsolver(void)
{
}


void GPIKsolver::solveOneStep(Skeleton* chain, const std::vector<Vector3_>& targets)
{
	for(int i = 6; i < m_mu.size(); ++i){
		chain->m_dim_values[i] = m_mu(i);
	}
	chain->update();
}