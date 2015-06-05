/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file SkeletonConverter.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "geometry/Skeleton.h"
#include "BVH.h"

namespace Etoile
{
	class SkeletonConverter
	{
	public:
		static void convertFromBVHToSkeleton(BVH* bvh, Skeleton* skeleton)
		{
			for(unsigned int i = 0; i < bvh->m_joints.size(); ++i)
			{
				BVH::Joint* joint = bvh->m_joints[i];
				Joint* j = new Joint(skeleton, joint->m_index_parent, joint->m_name);
				if(!bvh->m_isMeter)
				{
					skeleton->m_localTranslations.push_back(Vec3f(joint->m_offset[0] / 100.0,joint->m_offset[1] / 100.0,joint->m_offset[2] / 100.0));
				}
				else
				{
					skeleton->m_localTranslations.push_back(Vec3f(joint->m_offset[0],joint->m_offset[1],joint->m_offset[2]));
				}
				skeleton->m_globalPositions.push_back(Vec3f());
				skeleton->m_localRotations.push_back(Quaternionf());
				skeleton->m_globalOrientations.push_back(Quaternionf());
			}
			skeleton->update();
		}
	};
}