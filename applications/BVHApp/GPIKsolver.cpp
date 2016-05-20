/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GPIKsolver.cpp
* @brief 
* @date 1/2/2011
*/
#include "GPIKsolver.h"


GPIKsolver::GPIKsolver(Skeleton* sk)
{
	//loadConfig();
	if(sk != NULL)
		setSK(sk);
	m_maxTries = 1;
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

void GPIKsolver::setSK(Skeleton* sk)
{
	m_sk = sk;
	//computeGP();
}

void GPIKsolver::computeGP()
{
	for(unsigned int i = 0; i < m_mus.size(); ++i)
	{
		for(unsigned int j = 0; j < m_mus[i].size(); ++j)
		{
			m_sk->m_dim_values[j] = m_mus[i](j);
		}
		m_sk->update();

		m_targets.push_back( VectorX_::Zero(m_sk->m_endeffectors.size() * 3));
		std::vector<Vector3_> target;
		for(unsigned int j = 0; j < m_sk->m_endeffectors.size(); ++j)
		{
			int idx = m_sk->m_endeffectors[j];
			target.push_back(m_sk->m_joint_globalPositions[idx]);
			m_targets[i](j * 3) = target[j](0);
			m_targets[i](j * 3 + 1) = target[j](1);
			m_targets[i](j * 3 + 2) = target[j](2);
		}

	}
	buildK();
}

void GPIKsolver::buildK()
{
	int size = m_mus.size();
	m_k = MatrixX_::Zero(size, size);
	for(unsigned int i = 0 ; i < size; ++i)
	{
		for(unsigned int j = 0 ; j < size; ++j)
		{
			m_k(i,j) = kernel(m_targets[i], m_targets[j]);
		}
	}
	m_k_inverse = m_k.inverse();
	m_mu = VectorX_::Zero(size);
}

void GPIKsolver::computeASample(const VectorX_& target)
{
	VectorX_ kt = VectorX_::Zero(m_mus.size(), 1);
	for(unsigned int i = 0 ; i < m_mus.size(); ++i)
	{
		kt(i) = kernel(target, m_targets[i]);
	}
	VectorX_ v = kt.transpose() * m_k_inverse;

	int dim = m_mus[0].size();
	m_mu = VectorX_::Zero(dim);
	//double weight = 0;
	for(unsigned int i = 0 ; i < v.size(); ++i)
	{
		double v0 = v(i);
		m_mu = m_mu + m_mus[i]  * v0;
	}
}

void trimStringa( std::string& _string) 
{
	// Trim Both leading and trailing spaces
	size_t start = _string.find_first_not_of(" \t\r\n");
	size_t end   = _string.find_last_not_of(" \t\r\n");

	if(( std::string::npos == start ) || ( std::string::npos == end))
		_string = "";
	else
		_string = _string.substr( start, end-start+1 );
}

bool GPIKsolver::loadPoses(const std::string&  filepath)
{
	std::fstream in( filepath.c_str(), std::ios_base::in );

	if (!in.is_open() || !in.good())
	{
		std::cerr << "[GP] : cannot not open file "
			<< filepath
			<< std::endl;
		return false;
	}
#if defined(_DEBUG) || defined(DEBUG)
	std::cout<< "[GP] : start loading : "<< filepath <<std::endl;
#endif


	std::string line;
	std::getline(in,line);
	std::stringstream stream(line);
	int dim = 0;
	stream >> dim;
	int idx = 0; 
	while( in && !in.eof())
	{
		//lineIndex++;
		std::getline(in,line);
		if ( in.bad() ){
			std::cout << "  Warning! Could not read file properly! BVH part: skeleton"<<std::endl;
		}

		trimStringa(line);
		if ( line.size() == 0 || line[0] == '#' || isspace(line[0]) || line.empty() ) 
		{
			continue;
		}

		VectorX_ mu = VectorX_::Zero(dim);
		double value = 0;
		std::stringstream stream(line);
		for(unsigned int w = 0; w < dim; ++w)
		{
			stream >> value;
			mu(w) = value;
		}
		m_mus.push_back(mu);
		++idx;
	}
	double max = 100;
	double step = m_mus.size() / max;
	std::vector<VectorX_> mus;
	for(double i = 0; i < m_mus.size(); i += step)
	{
		mus.push_back(m_mus[int(i)]);
	}
	m_mus = mus;
	std::cout<<m_mus.size()<<std::endl;
	in.close();
#if defined(_DEBUG) || defined(DEBUG)
	std::cout<< "[GP] : loading is successful "<<std::endl;
#endif
	return true;
}