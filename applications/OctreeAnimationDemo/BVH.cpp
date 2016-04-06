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
#include <exception>
#include "Quaternion.h"

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

	
	void BVH::Joint::changeOrdertoZYX()
	{
		std::vector<int> indices;
		std::vector<Etoile::Vec3f> axis;
		for(int i = 0; i < m_dof; ++i)
		{
			int indx = i;
			if(m_dims[i].m_name== "Zrotation")
			{
				Etoile::Vec3f v(0,0,1);
				axis.push_back(v);
				indices.push_back(indx);
			}else if(m_dims[i].m_name== "Yrotation")
			{
				Etoile::Vec3f v(0,1,0);
				axis.push_back(v);
				indices.push_back(indx);
			}else if(m_dims[i].m_name== "Xrotation")
			{
				Etoile::Vec3f v(1,0,0);
				axis.push_back(v);
				indices.push_back(indx);
			}
		}
		if(indices.size() > 0)
		{
			m_dims[indices[0]].m_name = "Zrotation";
			m_dims[indices[1]].m_name = "Yrotation";
			m_dims[indices[2]].m_name = "Xrotation";

		
			for(int j = 0; j < p_owner->m_frames.size(); ++j){
				Frame& f = p_owner->m_frames[j];
				float value0 = f.m_values[m_dims[indices[0]].m_index] * 3.14159265 / 180.0;
				float value1 = f.m_values[m_dims[indices[1]].m_index] * 3.14159265 / 180.0;
				float value2 = f.m_values[m_dims[indices[2]].m_index] * 3.14159265 / 180.0;
				Etoile::Quaternionf q = Etoile::Quaternionf(axis[0], value0) * Etoile::Quaternionf(axis[1], value1) * Etoile::Quaternionf(axis[2], value2);
				Etoile::Vec3f v = q.getEulerAngleXYZBYAngle();
				f.m_values[m_dims[indices[0]].m_index] = v.z();
				f.m_values[m_dims[indices[1]].m_index] = v.y();
				f.m_values[m_dims[indices[2]].m_index] = v.x();
			}	
		}
	}

	void BVH::changeOrderToZYX()
	{
		for(unsigned int i = 0; i < m_joints.size(); ++i)
		{
			Joint* current = m_joints[i];
			current->changeOrdertoZYX();
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
//#if defined(_DEBUG) || defined(DEBUG)
//					std::cout<<"HIERARCHY " <<std::endl;
//#endif
				}
				else if(keyWrd == "ROOT")
				{
//#if defined(_DEBUG) || defined(DEBUG)
//					std::cout<<"Root " <<std::endl;
//#endif
					stream >> keyWrd;
					readJoint(in, keyWrd);
				}
				else if(keyWrd == "MOTION")
				{
//#if defined(_DEBUG) || defined(DEBUG)
//					std::cout<<"MOTION " <<std::endl;
//#endif
					stream >> keyWrd;
					readFrames(in);
				}
			}
		}

		for(unsigned int i = 0; i < m_joints.size(); ++i)
		{
			m_jointsId[m_joints[i]->m_name] = i;
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
//#if defined(_DEBUG) || defined(DEBUG)
//					std::cout<<"{ " <<std::endl;
//#endif
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
//#if defined(_DEBUG) || defined(DEBUG)
//					std::cout<<"OFFSET" <<std::endl;
//#endif
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
//#if defined(_DEBUG) || defined(DEBUG)
//					std::cout<<"CHANNELS" <<std::endl;
//#endif
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
						if(current->m_dims[i].m_name== "Zrotation")
						{
							current->m_dims[i].m_axis[0] = 0;
							current->m_dims[i].m_axis[1] = 0;
							current->m_dims[i].m_axis[2] = 1;
							
						}else if(current->m_dims[i].m_name== "Yrotation")
						{
							current->m_dims[i].m_axis[0] = 0;
							current->m_dims[i].m_axis[1] = 1;
							current->m_dims[i].m_axis[2] = 0;
						}else if(current->m_dims[i].m_name== "Xrotation")
						{
							current->m_dims[i].m_axis[0] = 1;
							current->m_dims[i].m_axis[1] = 0;
							current->m_dims[i].m_axis[2] = 0;
						}else
						{
							current->m_dims[i].m_axis[0] = 0;
							current->m_dims[i].m_axis[1] = 0;
							current->m_dims[i].m_axis[2] = 0;
						}
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
//#if defined(_DEBUG) || defined(DEBUG)
//					std::cout<<"{ " <<std::endl;
//#endif
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
//#if defined(_DEBUG) || defined(DEBUG)
//					std::cout<<"OFFSET" <<std::endl;
//#endif
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
				}

			}
		}

	}

	void BVH::readFrames(std::istream& in)
	{
		std::string line;
		std::string keyWrd;
		float v;
		{
			readLine(in,line);
			std::stringstream stream(line);
			stream >> keyWrd;
			if(!stream.fail())
			{
				if (keyWrd =="Frames:")
				{
					stream >> this->m_frameNb;
				}

			}
		}
		{
			readLine(in,line);
			std::stringstream stream(line);
			stream >> keyWrd;
			if(!stream.fail())
			{
				if (keyWrd =="Frame")
				{
					stream >> keyWrd;
					if(keyWrd == "Time:")
					{
						stream >> this->m_frametime;
					}
				}
			}
		}
		m_frames.resize(m_frameNb);
		for(int i = 0 ; i < this->m_frameNb; ++i)
		{
			readLine(in,line);
			std::stringstream stream(line);
			Frame& frame = m_frames[i];
			frame.m_values.resize(this->m_dims);
			for(int j = 0; j < m_dims; ++j)
			{
				stream >> frame.m_values[j];
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
		//write HIERARCHY
		std::vector<int> nb_children(m_joints.size());
		for(unsigned int i = 1; i < m_joints.size(); ++i)
		{
			++nb_children[m_joints[i]->m_index_parent];
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
					}else
					{
						break;
					}
					parent = m_joints[parent]->m_index_parent;
				}
			}
		}

		//write MOTION
		out<<"MOTION\n";
		out<<"Frames:\t"<<m_frames.size()<<"\n";
		out<<"Frame Time:\t"<<m_frametime<<"\n";
		for(int i = 0 ; i < m_frames.size(); ++i)
		{
			Frame& frame = m_frames[i];
			for(int j = 0; j < m_dims; ++j)
			{
				out<< frame.m_values[j] <<"  ";
			}
			out<<"\n";
		}
	}








	bool BVH::loadTextFile(const std::string& filepath)
	{
		m_filepath = filepath;
		std::fstream in(m_filepath.c_str(), std::ios_base::in );

		if (!in.is_open() || !in.good())
		{
			std::cerr << "[Skeleton] : cannot not open file "
				<< m_filepath
				<< std::endl;
			return false;
		}

	/*	{
#if defined(WIN32)
			std::string::size_type dot = m_filepath.find_last_of("\\/");
#else
			std::string::size_type dot = fileName.rfind("/");
#endif
			m_filepath = (dot == std::string::npos)
				? "./"
				: std::string(m_filepath.substr(0,dot+1));
		}*/

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
				Joint* j = new Joint(this, idxP, dof, name);

				stream >> j->m_offset[0];
				stream >> j->m_offset[1];
				stream >> j->m_offset[2];
				for(int i = 0; i < dof; ++i)
				{
					std::getline(in,line);
					std::stringstream stream(line);
					stream >> j->m_dims[i].m_axis[0];
					stream >> j->m_dims[i].m_axis[1];
					stream >> j->m_dims[i].m_axis[2];
					stream >> j->m_dims[i].m_limits[0];
					stream >> j->m_dims[i].m_limits[1];
					++lineNb;
				}
				
			}catch(int& e)
			{
				std::cout<<"SkeletonTextFileLoader: exception "<< lineNb<<" name "<<std::endl;
				continue;
			}
			++lineNb;
		}
		in.close();
		return true;
	}

	bool BVH::saveTextFile(const std::string& filepath)
	{
		if(m_joints.size() <= 0 ) return false;

		std::ofstream out;
		out.open(filepath);

		for(unsigned int i = 0; i < m_joints.size(); ++i)
		{
			Joint* joint = m_joints[i];
			if(joint->m_dof > 3)
				out<<joint->m_name<<" "<<joint->m_index_parent<<" "<<3<<" "<<joint->m_offset[0]/100.0<<" "<<joint->m_offset[1]/100.0<<" "<<joint->m_offset[2]/100.0<<std::endl;
			else
				out<<joint->m_name<<" "<<joint->m_index_parent<<" "<<joint->m_dof<<" "<<joint->m_offset[0]/100.0<<" "<<joint->m_offset[1]/100.0<<" "<<joint->m_offset[2]/100.0<<std::endl;
			for(int j = 0; j < joint->m_dof; ++j)
			{
				if(joint->m_dims[j].m_axis[0] == 0 && joint->m_dims[j].m_axis[1] == 0 && joint->m_dims[j].m_axis[2] == 0)
				{
				
				}else
					out<<joint->m_dims[j].m_axis[0]<<" "<<joint->m_dims[j].m_axis[1]<<" "<<joint->m_dims[j].m_axis[2]<<" "<<joint->m_dims[j].m_limits[0]<<" "<<joint->m_dims[j].m_limits[1]<<std::endl;
			}
		}


		out.close();
		return true;
	
	}


	void BVH::copy(BVH& bvh)
	{
		m_frameNb = bvh.m_frameNb;
		this->m_frames.clear();
		for(int n = 0 ; n <bvh.m_frames.size(); ++n)
		{
			Frame f;
			f.m_values.resize(this->m_dims);
			Frame bvf = bvh.m_frames[n];
			for(unsigned int i = 0; i < m_joints.size(); ++i)
			{
				Joint* joint = m_joints[i];
				Joint* bj = bvh.getJoint(joint->m_name);
				for(int j = 0; j < joint->m_dof; ++j)
				{
					Dim& dim = joint->m_dims[j];
					Dim& dimbv =bj->m_dims[j];
					f.m_values[dim.m_index] = bvf.m_values[dimbv.m_index];
				}
			}
			this->m_frames.push_back(f);
		}
		this->saveToBVHFile("bin/copy/newbvh.bvh");
	}