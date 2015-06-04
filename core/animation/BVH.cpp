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
		for(int i = 0; i < dof; ++i)
		{
			m_dims[i].m_index = bvh->m_dims;	
			++bvh->m_dims;
		}
	}

	BVH::BVH()
	{

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
		m_index.push(m_index.size());
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
					for(int i = 0; i < dim; ++i)
					{
						stream >> keyWrd;
						current->m_dims[i].m_name = keyWrd;
					}

				}
			}
		}


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
				
				}
				else if(keyWrd =="}")
				{
				
				}
			}
		}



	}

	// need to find the load problem, file need empty space or /n to read eachline,
	// solved by linechar[sizeLine - 1] = ' ';
	void BVH::loadSkeleton(std::istream& in)
	{
		int lineIndex = 0;
		std::string line;

		Joints jointlist;
		int popJointId = -1;
		Joint* currentJoint = NULL;

		BVHFrame frame;
		std::string _incre = "+";
#if defined(_DEBUG) || defined(DEBUG)
		std::cout<< "[BVH] : start loading skeleton " <<std::endl;
#endif
		while( in && !in.eof() )
		{
			lineIndex++;
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

			if(keyWrd == "HIERARCHY")
			{
#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"HIERARCHY " <<std::endl;
#endif
			}
			else if ( keyWrd =="{")
			{
#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"{ " <<std::endl;
#endif
				jointlist.push_back(currentJoint);
			}
			else if ( keyWrd== "}")
			{
#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"} " <<std::endl;
#endif
				Joint* joint = jointlist.back();
				jointlist.pop_back();
				popJointId = joint->getParentId();
			}
			else if ( ( keyWrd == "ROOT" ) || ( keyWrd == "JOINT" ) )
			{
#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"add Joint: ";
#endif
				std::string name;
				stream >> name;

				int id = _pSkeleton->createJoint(name, popJointId);
				std::cout<<name;
				currentJoint = _pSkeleton->getJoint(id);
				popJointId = id;
				if(id == 0){_rootName = name;}

			}
			else if ( keyWrd == "End" )
			{
#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"add Joint: ";
#endif
				std::string name;
				stream >> name;
				_incre += "+";
				std::cout<< name + _incre;
				int id = _pSkeleton->createJoint(name + _incre, popJointId);
				currentJoint = _pSkeleton->getJoint(id);
				popJointId = id;
			}
			else if ( keyWrd == "OFFSET" )
			{
#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"offset: ";
#endif
				float x, y, z;
				stream >> x;
				stream >> y;
				stream >> z;
#if defined(_DEBUG) || defined(DEBUG)
				std::cout<< x <<" " << y << " "<< z <<" ";
#endif
				currentJoint->setLocalPosition(Vec3f(x,y,z));
				currentJoint->update();
			}
			else if ( keyWrd == "CHANNELS" )
			{
#if defined(_DEBUG) || defined(DEBUG)
				std::cout<<"channels: ";
#endif
				JointMotion jointmotion;
				jointmotion._jointName = currentJoint->getName();

				int nbChannels;
				stream >> nbChannels;
				std::cout<<nbChannels<<" "<<std::endl;
				for (int i = 0; i < nbChannels; i++ )
				{

					Channel  channel;
					channel._value = 0;

					stream >> keyWrd;
					if ( keyWrd == "Xrotation" )
						channel._type = X_ROTATION;
					else if ( keyWrd == "Yrotation" )
						channel._type = Y_ROTATION;
					else if ( keyWrd == "Zrotation" )
						channel._type = Z_ROTATION;
					else if ( keyWrd == "Xposition" )
						channel._type = X_POSITION;
					else if ( keyWrd == "Yposition" )
						channel._type = Y_POSITION;
					else if ( keyWrd == "Zposition" )
						channel._type = Z_POSITION;

					jointmotion._channels.push_back(channel);
				}
				frame.addJointMotion(jointmotion);
			}
			else if ( keyWrd == "MOTION" )
			{
#if defined(_DEBUG) || defined(DEBUG)
				std::cout<< "[BVH] : start loading frames " <<std::endl;
#endif
				loadFrames(in, frame);
				break;
			}
			//delete[] linechar;

		}
	}


	void BVH::loadFrames(std::istream& in, BVHFrame& frame)
	{
		std::string line;

		int num_frame = 0;
		{
			std::getline(in,line);
			if ( in.bad() ){
				std::cout << "  Warning! Could not read file properly! BVH part: Frames"<<std::endl;
			}
			trimString(line);
			while ( line.size() == 0 || line[0] == '#' || isspace(line[0]) || line.empty() ) {
				std::getline(in,line);
				if ( in.bad() ){
					std::cout << "  Warning! Could not read file properly! BVH part: Frames"<<std::endl;
				}
				trimString(line);
			}
			std::string keyWrd;
			std::stringstream stream(line);
			stream >> keyWrd;

			if (string::npos == keyWrd.find("Frames")) return;
			stream >> num_frame;
#if defined(_DEBUG) || defined(DEBUG)
			std::cout<< "frames : "<< num_frame <<std::endl;
#endif
			//delete []linechar;
		}

		float interval = 0;
		{
			std::getline(in,line);
			std::string keyWrd;
			std::stringstream stream(line);
			stream >> keyWrd;
			if(keyWrd == "Frame")
			{

			}
			stream >> keyWrd;
			if (string::npos == keyWrd.find("Time")) 
			{

			}
			stream >> interval;
#if defined(_DEBUG) || defined(DEBUG)
			std::cout<< "interval : "<< interval <<std::endl;
#endif
			//delete []linechar;
		}


		//_skeletonframes->setInterval(interval);

		for(int i = 0; i < num_frame; ++i)
		{

			std::getline(in,line);
			std::stringstream stream(line);

			for(unsigned j = 0; j < frame.getJointMotions().size(); ++j)
			{
				JointMotion& motion = frame.getJointMotionByIndex(j);
				for(unsigned k = 0; k < motion._channels.size(); ++k)
				{
					stream >> motion._channels[k]._value;
				}
			}
			KeyFrame* sframe = new KeyFrame(i * interval);
			sframe->setRootJoint(_rootName);
			fromBVHFrameToSkeletonFrame(frame, *sframe);
			_skeletonframes->insertKeyFrame(sframe);
			//delete []linechar;
		}

	}


	void BVH::fromBVHFrameToSkeletonFrame(BVHFrame& frame, KeyFrame& skeletonframe)
	{
		JointMotions& motions = frame.getJointMotions();
		for(unsigned int i = 0; i < motions.size(); ++i)
		{
			JointMotion& motion = motions[i];
			std::string name = motion._jointName;
			//Joint* joint = _pSkeleton->getJoint(name);
			Quaternionf rotation;
			Vec3f position;
			for(unsigned int j = 0; j < motion._channels.size(); ++j)
			{
				Channel& ch = motion._channels[j];
				ChannelType type = ch._type;
				float rotationValue = ch._value / 180.0 * 3.14159265;

				if (type == X_POSITION)
				{
					position[0] = ch._value;
				}
				else if (type == Y_POSITION)
				{
					position[1] = ch._value;
				}
				else if (type == Z_POSITION)
				{
					position[2] = ch._value;
				}
				else if (type == X_ROTATION)
				{
					Quaternionf q;
					q.setAxisAngle(Vec3f(1,0,0), rotationValue);
					//rotation =  q * rotation;
					rotation =  rotation * q;
				}
				else if (type == Y_ROTATION)
				{
					Quaternionf q;
					q.setAxisAngle(Vec3f(0,1,0), rotationValue);
					//rotation =  q * rotation;
					rotation =  rotation * q;

				}
				else if (type == Z_ROTATION)
				{
					Quaternionf q;
					q.setAxisAngle(Vec3f(0,0,1), rotationValue);
					//rotation =  q * rotation;
					rotation =  rotation * q;
				}

			}
			//        joint->setOriginalPosition(position);
			//        joint->rotate(rotation);
			//        joint->updateLocally();

			FrameParameters info;
			info._localRotation = rotation;
			info._translation = position;

			skeletonframe.addFrameParameters(name, info);
		}
	}



}
