/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file BVH.cpp
* @brief 
* @date 1/2/2011
*/

#include "BVH.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

namespace Etoile
{
	void BVH::trimString( std::string& _string) 
	{
		// Trim Both leading and trailing spaces
		size_t start = _string.find_first_not_of(" \t\r\n");
		size_t end   = _string.find_last_not_of(" \t\r\n");

		if(( std::string::npos == start ) || ( std::string::npos == end))
			_string = "";
		else
			_string = _string.substr( start, end-start+1 );
	}

	BVH::Joint::Joint(BVH* bvh, int parent, int dof, const std::string& name)
	{
		m_isleaf = false;
		p_owner = bvh;
		m_index = bvh->m_joints.size();
		bvh->m_joints.push_back(this);
		m_index_parent = parent;
		m_name = name;
		m_dof = dof;
		m_dims.resize(m_dof);
		m_childrenNb = 0;
		for(int i = 0; i < dof; ++i)
		{
			m_dims[i].m_index = bvh->m_dims;	
			++bvh->m_dims;
		}
	}

	BVH::Joint::Joint(BVH* bvh, int parent, const std::string& name)
	{
		m_isleaf = false;
		p_owner = bvh;
		m_index = bvh->m_joints.size();
		bvh->m_joints.push_back(this);
		m_index_parent = parent;
		m_name = name;
		m_childrenNb = 0;
	}

	void BVH::Joint::init(int dof)
	{
		m_dof = dof;
		m_dims.resize(m_dof);
		for(int i = 0; i < dof; ++i)
		{
			m_dims[i].m_index = p_owner->m_dims;	
			++p_owner->m_dims;
		}
	}

	bool BVH::loadFromBVHFile(const std::string& filepath)
	{
		m_filepath = filepath;
		std::fstream in( m_filepath.c_str(), std::ios_base::in );

		if (!in.is_open() || !in.good())
		{
			std::cerr << "[BVH] : cannot not open file "
				<< m_filepath
				<< std::endl;
			return false;
		}
#if defined(_DEBUG) || defined(DEBUG)
		std::cout<< "[BVH] : start loading : "<< filepath <<std::endl;
#endif
		read(in);
		in.close();
#if defined(_DEBUG) || defined(DEBUG)
		std::cout<< "[BVH] : loading is successful "<<std::endl;
#endif
		return true;

	}

	void BVH::read(std::istream& in)
	{
		//int lineIndex = 0;
		std::string line;
		while( in && !in.eof() )
		{
			//lineIndex++;
			std::getline(in,line);
			if ( in.bad() ){
				std::cout << "  Warning! Could not read file properly! BVH part: skeleton"<<std::endl;
			}

			trimString(line);
			if ( line.size() == 0 || line[0] == '#' || isspace(line[0]) || line.empty() ) 
			{
				continue;
			}

			std::string keyWrd;
			std::stringstream stream(line);
			stream >> keyWrd;
			if(!stream.fail())
			{
				if(keyWrd == "HIERARCHY")
				{
#if defined(_DEBUG) || defined(DEBUG)
					std::cout<<"HIERARCHY " <<std::endl;
#endif
				}
				else if(keyWrd == "ROOT")
				{
#if defined(_DEBUG) || defined(DEBUG)
					std::cout<<"Root " <<std::endl;
#endif
					stream >> keyWrd;
					readJoint(in, keyWrd);
				}
			}
		}
	}

	void BVH::readLine(std::istream& in, std::string& line)
	{
		std::getline(in,line);
		if ( in.bad() ){
			std::cout << "  Warning! Could not read file properly! BVH part: joint"<<std::endl;
			assert(0);
		}

		trimString(line);
	}


	void BVH::readJoint(std::istream& in, const std::string& name)
	{
		int parent = m_index.size() > 0 ? m_index.top() : -1;

		std::string line;
		std::string keyWrd;
		float x,y,z;
		int dim;
		Joint* current;

		{
			readLine(in,line);
			std::stringstream stream(line);
			stream >> keyWrd;
			if(!stream.fail())
			{
				if ( keyWrd =="{")
				{
#if defined(_DEBUG) || defined(DEBUG)
					std::cout<<"{ " <<std::endl;
#endif
				}
			}
		}

		{
			readLine(in,line);
			std::stringstream stream(line);
			stream >> keyWrd;
			if(!stream.fail())
			{
				if (keyWrd =="OFFSET")
				{
#if defined(_DEBUG) || defined(DEBUG)
					std::cout<<"OFFSET" <<std::endl;
#endif
					stream >> x;
					stream >> y;
					stream >> z;
				}
			}
		}

		{
			readLine(in,line);
			std::stringstream stream(line);
			stream >> keyWrd;
			if(!stream.fail())
			{
				if (keyWrd =="CHANNELS")
				{
#if defined(_DEBUG) || defined(DEBUG)
					std::cout<<"CHANNELS" <<std::endl;
#endif
					stream >> dim;
					current = new Joint(this, parent, dim, name);
					current->m_offset[0] = x;
					current->m_offset[1] = y;
					current->m_offset[2] = z;
					current->m_level = m_index.size();
					for(int i = 0; i < dim; ++i)
					{
						stream >> keyWrd;
						current->m_dims[i].m_name = keyWrd;
					}

				}
			}
		}

		m_index.push(current->m_index);
		bool loop = true;
		{
			while(loop)
			{
				readLine(in,line);
				std::stringstream stream(line);
				stream >> keyWrd;
				if(!stream.fail())
				{
					if (keyWrd =="JOINT")
					{
						stream >> keyWrd;
						readJoint(in, keyWrd);
					}
					else if(keyWrd =="End")
					{
						stream >> keyWrd;
						readEnd(in, keyWrd);
					}if (keyWrd =="}")
					{
						loop = false;
						m_index.pop();
						if(parent > -1)
							m_joints[parent]->m_childrenNb++;
					}

				}
			}
		}



	}



	void BVH::readEnd(std::istream& in, const std::string& name)
	{
		int parent = m_index.size() > 0 ? m_index.top() : -1;
		std::string line;
		std::string keyWrd;
		float x,y,z;
		int dim;
		Joint* current;

		{
			readLine(in,line);
			std::stringstream stream(line);
			stream >> keyWrd;
			if(!stream.fail())
			{
				if ( keyWrd =="{")
				{
#if defined(_DEBUG) || defined(DEBUG)
					std::cout<<"{ " <<std::endl;
#endif
				}
			}
		}

		{
			readLine(in,line);
			std::stringstream stream(line);
			stream >> keyWrd;
			if(!stream.fail())
			{
				if (keyWrd =="OFFSET")
				{
#if defined(_DEBUG) || defined(DEBUG)
					std::cout<<"OFFSET" <<std::endl;
#endif
					stream >> x;
					stream >> y;
					stream >> z;

					current = new Joint(this, parent, 0, name);
					current->m_offset[0] = x;
					current->m_offset[1] = y;
					current->m_offset[2] = z;
					current->m_isleaf = true;
					current->m_level = m_index.size();
				}
			}
		}

		{
			readLine(in,line);
			std::stringstream stream(line);
			stream >> keyWrd;
			if(!stream.fail())
			{
				if (keyWrd =="}")
				{
					//m_index.pop();
					if(parent > -1)
						m_joints[parent]->m_childrenNb++;
				}

			}
		}

	}




	bool BVH::saveToBVHFile(const std::string& filepath)
	{
		if(m_joints.size() <= 0 ) return false;

		std::ofstream out;
		out.open(filepath);
#if defined(_DEBUG) || defined(DEBUG)
		std::cout<< "[BVH] : start writing : "<< filepath <<std::endl;
#endif
		write(out);
		out.close();
#if defined(_DEBUG) || defined(DEBUG)
		std::cout<< "[BVH] : writing is successful "<<std::endl;
#endif
		return true;
	}

	void BVH::write(std::ostream& out)
	{
		std::vector<int> nb_children;
		for(unsigned int i = 0; i < m_joints.size(); ++i)
		{
			nb_children.push_back(m_joints[i]->m_childrenNb);
		}

		std::string incent = "\t";
		out<<"HIERARCHY\n";
		Joint* current = m_joints[0];
		out<<"ROOT "<<current->m_name<<"\n";
		out<<"{\n";
		out<<incent<<"OFFSET "<<current->m_offset[0]<<" "<<current->m_offset[1]<<" "<<current->m_offset[2]<<"\n";
		out<<incent<<"CHANNELS "<<current->m_dof<<" ";
		for(int j = 0; j < current->m_dof; ++j)
		{
			out<<current->m_dims[j].m_name<<" ";
		}
		out<<"\n";

		for(unsigned int i = 1; i < m_joints.size(); ++i)
		{
			Joint* current = m_joints[i];
			int parent = current->m_index_parent;
			--nb_children[parent];
			if(current->m_dof > 0)
			{
				out<< std::string(current->m_level,'\t')  <<"JOINT "<<current->m_name<<"\n";
				out<< std::string(current->m_level,'\t')  <<"{\n";
				out<< std::string(current->m_level,'\t')  <<incent<<"OFFSET "<<current->m_offset[0]<<" "<<current->m_offset[1]<<" "<<current->m_offset[2]<<"\n";
				out<< std::string(current->m_level,'\t')  <<incent<<"CHANNELS "<<current->m_dof<<" ";
				for(int j = 0; j < current->m_dof; ++j)
				{
					out<<current->m_dims[j].m_name<<" ";
				}
				out<<"\n";
			}else
			{
				out<< std::string(current->m_level,'\t')  <<"End "<<current->m_name<<"\n";
				out<< std::string(current->m_level,'\t')  <<"{\n";
				out<< std::string(current->m_level,'\t')  <<incent<<"OFFSET "<<current->m_offset[0]<<" "<<current->m_offset[1]<<" "<<current->m_offset[2]<<"\n";
				out<< std::string(current->m_level,'\t')  <<"}\n";

				while(parent >= 0)
				{
					if(nb_children[parent] == 0)
					{
						out<< std::string( m_joints[parent]->m_level,'\t')  <<"}\n";
					}
					parent = m_joints[parent]->m_index_parent;
				}
			}
		}
	}

}
