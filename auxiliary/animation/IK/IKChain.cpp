/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Skeleton.cpp
* @brief 
* @date 1/2/2011
*/

#include "IKChain.h"
#include <fstream>
#include <exception>

/**
* @brief For tracking memory leaks under windows using the crtdbg
*/
#if ( defined( _DEBUG ) || defined( DEBUG ) ) && defined( _MSC_VER )
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW
#endif

namespace Etoile
{

	IKChain::Joint::Joint(IKChain* sk, int parent, int dof, const std::string& name)
	{
		p_owner = sk;
		m_index = sk->m_joints.size();
		sk->m_joints.push_back(this);
		m_index_parent = parent;
		m_name = name;
		m_dof = dof;

		m_dims.resize(m_dof);
		for(int i = 0; i < m_dof; ++i)
		{
			
			m_dims[i].m_idx = sk->m_dims.size();
			if(i == 0)
			{
				if( parent >= 0)
				{
					m_dims[i].m_lastIdx = sk->m_joints[parent]->m_dims.back().m_idx;
				}
				else
				{
					m_dims[i].m_lastIdx = -1;
				}
			}else
			{
				m_dims[i].m_lastIdx = m_dims[i - 1].m_idx;
			}

			sk->m_dims.push_back(&m_dims[i]);
			sk->m_dim_axis.push_back(Vector3_::Zero());
			sk->m_dim_anglelimites.push_back(Vector2_(-3.14159265,3.14159265));
			sk->m_dim_localRotations.push_back(Matrix3_::Identity());
			sk->m_dim_globalOrientations.push_back(Matrix3_::Identity());
			sk->m_dim_localTranslations.push_back(Vector3_::Zero());
			sk->m_dim_globalPositions.push_back(Vector3_::Zero());
			sk->m_dim_values.push_back(0);
			sk->m_average_values.push_back(0);
			sk->m_dedr_max.push_back(Vector3_(100,100,100));
			sk->m_dedr_min.push_back(Vector3_(-100,-100,-100));
			sk->m_posture_variation.push_back(1);
			
		}
		sk->m_joint_localRotations.push_back(Matrix3_::Identity());
		sk->m_joint_globalOrientations.push_back(Matrix3_::Identity());
		sk->m_joint_localTranslations.push_back(Vector3_::Zero());
		sk->m_joint_globalPositions.push_back(Vector3_::Zero());
		
	}

	void IKChain::reset()
	{
		for(int i = 0; i < m_dims.size();++i)
		{
			m_dim_localRotations[i].setIdentity();
			m_dim_globalOrientations[i].setIdentity();
			m_dim_anglelimites[i] = Vector2_(-3.14159265,3.14159265);
			m_dim_values[i] = 0;
			m_average_values[i] = 0;
			m_dedr_max[i] = Vector3_(100,100,100);
			m_dedr_min[i] = Vector3_(-100,-100,-100);
			m_posture_variation[i] = 1;//0.0001;
		}
		update();
	}

	bool IKChain::loadFromFile(const std::string& fileName)
	{
		std::fstream in(fileName.c_str(), std::ios_base::in );

		if (!in.is_open() || !in.good())
		{
			std::cerr << "[SkeletonTextFileLoader] : cannot not open file "
				<< fileName
				<< std::endl;
			return false;
		}

		{
#if defined(WIN32)
			std::string::size_type dot = fileName.find_last_of("\\/");
#else
			std::string::size_type dot = fileName.rfind("/");
#endif
			/*m_path = (dot == std::string::npos)
			? "./"
			: std::string(fileName.substr(0,dot+1));*/
		}

		read(in);

		in.close();
		return true;
	}

	void IKChain::read(std::istream& in)
	{
		std::string line;
		int lineNb = 0;
		while( in && !in.eof() )
		{
			std::getline(in,line);
			if ( in.bad() ){
				std::cout << "  Warning! Could not read file properly!"<<std::endl;
			}

			if ( line.size() == 0 || line[0] == '#' || isspace(line[0]) || line.empty() ) {
				continue;
			}

			std::stringstream stream(line);
			try
			{
				std::string name;
				stream >> name;
				int idxP;
				stream >> idxP;
				int dof;
				stream >> dof;
				double x,y,z;
				stream >> x;
				stream >> y;
				stream >> z;
				Joint* j = new Joint(this, idxP, dof, name);
				m_dim_localTranslations[j->m_dims[0].m_idx] = Vector3_(x,y,z);
				m_joint_localTranslations[j->m_index] = Vector3_(x,y,z);
				//std::cout<< "Joint: "<<name <<" dof: "<<dof<< " parent: "<<idxP<<" localTrans: "<< m_localTranslations[j->m_dims[0].m_idx].transpose() <<std::endl;
				for(int i = 0; i < dof; ++i)
				{
					std::getline(in,line);
					/*if ( line.size() == 0 || line[0] == '#' || isspace(line[0]) || line.empty() ) {
						continue;
					}*/
					std::stringstream stream(line);

					double x,y,z;
					stream >> x;
					stream >> y;
					stream >> z;
					m_dim_axis[j->m_dims[i].m_idx] = Vector3_(x,y,z);
					//std::cout<<i<< " axis: "<<m_axis[j->m_dims[i].m_idx].transpose()<<std::endl;

					double minV,maxV;
					stream >> minV;
					stream >> maxV;
					if(!stream.fail())
					{
						m_dim_anglelimites[j->m_dims[i].m_idx] = Vector2_(minV,maxV);
						//std::cout<<" limits: "<<m_anglelimites[j->m_dims[i].m_idx].transpose()<<std::endl;
					}
				}

			}catch(std::exception& e)
			{
				std::cout<<"IKChain: exception "<< lineNb<<" name "<<std::endl;
				continue;
			}
			++lineNb;
		}
		this->updateAllDims();
		this->update();
	}

	void IKChain::draw(int type)
	{
		if(type == 0)
		{
			for(int i = 0; i < m_joints.size(); ++i)
			{
			
			}
		}
		else if(type == 1)
		{
		
		}
	}
}
