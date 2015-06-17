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
				Joint* j = new Joint(skeleton, joint->m_index_parent, joint->m_dof, joint->m_name);
				if(!bvh->m_isMeter)
				{
					skeleton->m_localTranslations[i] = (Vec3f(joint->m_offset[0] / 100.0,joint->m_offset[1] / 100.0,joint->m_offset[2] / 100.0));
				}
				else
				{
					skeleton->m_localTranslations[i] = (Vec3f(joint->m_offset[0],joint->m_offset[1],joint->m_offset[2]));
				}
			}
			skeleton->update();
		}

		static float degreeToRadian(float v)
		{
			return v * 3.14159265 / 180;
		}

		static float radianToDegree(float v)
		{
			return v * 180 / 3.14159265;
		}

		static void updateBVHFrameToSkeleton(BVH* bvh, Skeleton* skeleton, int framenb)
		{
			Frame& frame = bvh->m_frames[framenb];
			for(unsigned int i = 0; i < bvh->m_joints.size(); ++i)
			{
				BVH::Joint* joint = bvh->m_joints[i];
				Joint* jsk = skeleton->m_joints[i];
				Quaternionf q;
				for(unsigned int j = 0; j < joint->m_dims.size(); ++j)
				{
					BVH::Dim& dim = joint->m_dims[j];
					float v = frame.m_values[dim.m_index];
					if(dim.m_name == "Xposition")
					{
						skeleton->m_localTranslations[i][0] = bvh->m_isMeter ? v : v / 100.0;
					}
					else if(dim.m_name == "Yposition")
					{
						skeleton->m_localTranslations[i][1] = bvh->m_isMeter ? v : v / 100.0;
					}
					else if(dim.m_name == "Zposition")
					{
						skeleton->m_localTranslations[i][2] = bvh->m_isMeter ? v : v / 100.0;
					}
					else if(dim.m_name == "Zrotation")
					{
						q = q * Quaternionf(Vec3f(0,0,1),degreeToRadian(v));
					}
					else if(dim.m_name == "Xrotation")
					{
						q = q * Quaternionf(Vec3f(1,0,0),degreeToRadian(v));
					}
					else if(dim.m_name == "Yrotation")
					{
						q = q * Quaternionf(Vec3f(0,1,0),degreeToRadian(v));
					}
				}
				skeleton->m_localRotations[i] = q;
			}
			skeleton->update();
		}

		
		static void updateSkeletonToBVHFrame(Skeleton* skeleton, BVH* bvh,  int framenb)
		{
			Frame& frame = bvh->m_frames[framenb];
			for(unsigned int i = 0; i < bvh->m_joints.size(); ++i)
			{
				BVH::Joint* joint = bvh->m_joints[i];
				Joint* jsk = skeleton->m_joints[i];
				int d3 = 0; 
				for(unsigned int j = 0; j < joint->m_dims.size(); ++j)
				{
					BVH::Dim& dim = joint->m_dims[j];
					if(dim.m_name == "Zrotation" || dim.m_name == "Xrotation" || dim.m_name == "Yrotation")
					{
						frame.m_values[dim.m_index] = jsk->m_values[d3];
						++d3;
					}
				}
			}
		}

		static void updateBVHFrameToRelativeBodySkeleton(BVH* bvh, Skeleton* skeleton, int framenb)
		{
			Frame& frame = bvh->m_frames[framenb];
			for(unsigned int i = 1; i < bvh->m_joints.size(); ++i)
			{
				BVH::Joint* joint = bvh->m_joints[i];
				Joint* jsk = skeleton->m_joints[i];
				Quaternionf q;
				for(unsigned int j = 0; j < joint->m_dims.size(); ++j)
				{
					BVH::Dim& dim = joint->m_dims[j];
					float v = frame.m_values[dim.m_index];
					if(dim.m_name == "Xposition")
					{
						skeleton->m_localTranslations[i][0] = bvh->m_isMeter ? v : v / 100.0;
					}
					else if(dim.m_name == "Yposition")
					{
						skeleton->m_localTranslations[i][1] = bvh->m_isMeter ? v : v / 100.0;
					}
					else if(dim.m_name == "Zposition")
					{
						skeleton->m_localTranslations[i][2] = bvh->m_isMeter ? v : v / 100.0;
					}
					else if(dim.m_name == "Zrotation")
					{
						q = q * Quaternionf(Vec3f(0,0,1),degreeToRadian(v));
					}
					else if(dim.m_name == "Xrotation")
					{
						q = q * Quaternionf(Vec3f(1,0,0),degreeToRadian(v));
					}
					else if(dim.m_name == "Yrotation")
					{
						q = q * Quaternionf(Vec3f(0,1,0),degreeToRadian(v));
					}
				}
				skeleton->m_localRotations[i] = q;
			}
			skeleton->update();
		}

	};
}