// BVHConversion.cpp : Defines the entry point for the console application.
//

#include <string>
#include <iostream>
#include <fstream>
#include "animation/SkeletonConverter.h"
using namespace Etoile;

/*std::vector<std::string> getNames()
{
	std::vector<std::string> name;
	name.push_back("hip");
	name.push_back("rHand");
	name.push_back("lHand");
	name.push_back("rFoot");
	name.push_back("lFoot");
	return name;
}*/

void createBVHDataFile(BVH& bvh, const std::string& filepath)
{
	std::ofstream out;
	out.open(filepath);

	for(unsigned int i = 0; i < bvh.m_joints.size(); ++i)
	{
		BVH::Joint* joint = bvh.m_joints[i];
		std::string name = joint->m_name;
		out<<name<<"_x, "<<name<<"_y, "<<name<<"_z, ";
	}
	for(unsigned int i = 0; i < bvh.m_joints.size(); ++i)
	{
		BVH::Joint* joint = bvh.m_joints[i];
		for(unsigned int j = 0; j < joint->m_dims.size(); ++j)
		{
			BVH::Dim& dim = joint->m_dims[j];
			out<<joint->m_name<<"_"<<dim.m_name<<", ";
		}
	}
	out<<" \n";

	Skeleton* sk = new Skeleton("s");
	SkeletonConverter::convertFromBVHToSkeleton(&bvh, sk);

	float scale = 100;
	for(unsigned int i = 0; i < bvh.m_frames.size(); ++i)
	{
		Frame& f = bvh.m_frames[i];
		SkeletonConverter::updateBVHFrameToSkeleton(&bvh, sk, i);
		for(int j = 0; j < bvh.m_joints.size(); ++j)
		{
			std::string name = bvh.m_joints[j]->m_name;
			Vec3f& pos = sk->m_globalPositions[sk->getJoint(name)->m_index];
			out<<pos.x() * scale<<", "<<pos.y() * scale<<", "<<pos.z() * scale<<", ";
		}
		for(unsigned int j = 0; j < f.m_values.size(); ++j)
		{
			out<< f.m_values[j]<<", ";
		}
		out<<" \n";
	}

	out.close();
}

void createRelativeBodyBVHDataFile(BVH& bvh, const std::string& filepath)
{
	std::ofstream out;
	out.open(filepath);

	for(unsigned int i = 0; i < bvh.m_joints.size(); ++i)
	{
		BVH::Joint* joint = bvh.m_joints[i];
		std::string name = joint->m_name;
		out<<name<<"_x, "<<name<<"_y, "<<name<<"_z, ";
	}
	for(unsigned int i = 0; i < bvh.m_joints.size(); ++i)
	{
		BVH::Joint* joint = bvh.m_joints[i];
		for(unsigned int j = 0; j < joint->m_dims.size(); ++j)
		{
			BVH::Dim& dim = joint->m_dims[j];
			out<<joint->m_name<<"_"<<dim.m_name<<", ";
		}
	}
	out<<" \n";

	Skeleton* sk = new Skeleton("s");
	SkeletonConverter::convertFromBVHToSkeleton(&bvh, sk);

	float scale = 100;
	for(unsigned int i = 0; i < bvh.m_frames.size(); ++i)
	{
		Frame& f = bvh.m_frames[i];
		SkeletonConverter::updateBVHFrameToRelativeBodySkeleton(&bvh, sk, i);
		for(int j = 0; j < bvh.m_joints.size(); ++j)
		{
			std::string name = bvh.m_joints[j]->m_name;
			Vec3f& pos = sk->m_globalPositions[sk->getJoint(name)->m_index];
			out<<pos.x() * scale<<", "<<pos.y() * scale<<", "<<pos.z() * scale<<", ";
		}
		for(unsigned int j = 0; j < f.m_values.size(); ++j)
		{
			out<< f.m_values[j]<<", ";
		}
		out<<" \n";
	}

	out.close();
}
