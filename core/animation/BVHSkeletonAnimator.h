/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file BVHSkeletonAnimator.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "AnimationManager.h"
#include "BVH.h"
#include "geometry/Skeleton.h"

namespace Etoile
{
	class BVHSkeletonAnimator : public Animator
	{
		BVH* p_bvh;
		Skeleton* p_sk;
		int m_frameIndex;

		float degreeToRadian(float v)
		{
			return v * 3.14159265 / 180;
		}
	public:
		BVHSkeletonAnimator(BVH* bvh, Skeleton* sk):Animator()
		{
			p_bvh = bvh;
			p_sk = sk;
			m_frameIndex = 0;
		}

		virtual void update()
		{
			Frame& frame = p_bvh->m_frames[m_frameIndex];
			for(unsigned int i = 0; i < p_bvh->m_joints.size(); ++i)
			{
				BVH::Joint* joint = p_bvh->m_joints[i];
				Joint* jsk = p_sk->m_joints[i];
				Quaternionf q;
				for(unsigned int j = 0; j < joint->m_dims.size(); ++j)
				{
					BVH::Dim& dim = joint->m_dims[j];
					float v = frame.m_values[dim.m_index];
					if(dim.m_name == "Xposition")
					{
						p_sk->m_localTranslations[i][0] = p_bvh->m_isMeter ? v : v / 100.0;
					}
					else if(dim.m_name == "Yposition")
					{
						p_sk->m_localTranslations[i][1] = p_bvh->m_isMeter ? v : v / 100.0;
					}
					else if(dim.m_name == "Zposition")
					{
						p_sk->m_localTranslations[i][2] = p_bvh->m_isMeter ? v : v / 100.0;
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
				p_sk->m_localRotations[i] = q;
			}
			p_sk->update();

			++m_frameIndex;
			if(m_frameIndex >= p_bvh->m_frames.size())
			{
				m_frameIndex = 0;
			}
		}

		virtual void update(int idx)
		{
			if(idx >= p_bvh->m_frames.size() || idx < 0) return;
			Frame& frame = p_bvh->m_frames[idx];
			for(unsigned int i = 0; i < p_bvh->m_joints.size(); ++i)
			{
				BVH::Joint* joint = p_bvh->m_joints[i];
				Joint* jsk = p_sk->m_joints[i];
				Quaternionf q;
				for(unsigned int j = 0; j < joint->m_dims.size(); ++j)
				{
					BVH::Dim& dim = joint->m_dims[j];
					float v = frame.m_values[dim.m_index];
					if(dim.m_name == "Xposition")
					{
						p_sk->m_localTranslations[i][0] = p_bvh->m_isMeter ? v : v / 100.0;
					}
					else if(dim.m_name == "Yposition")
					{
						p_sk->m_localTranslations[i][1] = p_bvh->m_isMeter ? v : v / 100.0;
					}
					else if(dim.m_name == "Zposition")
					{
						p_sk->m_localTranslations[i][2] = p_bvh->m_isMeter ? v : v / 100.0;
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
				p_sk->m_localRotations[i] = q;
			}
			p_sk->update();
		}

	};
}