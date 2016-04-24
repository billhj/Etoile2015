/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file BVH.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <string>
#include <vector>
#include <stack>
#include <map>

#include "Skeleton.h"

	struct Frame
	{
		std::vector<float> m_values;
	};

	struct BVH
	{

		//Frame createFrame()
		//{
		//	Frame f;
		//	f.m_values.resize(m_dims);
		//	for(unsigned int i = 0; i < m_joints.size(); ++i)
		//	{
		//		Joint* joint = m_joints[i];
		//		for(int j = 0; j < joint->m_dof; ++j)
		//		{
		//			Dim& dim = joint->m_dims[j];
		//			//if(dim.m_value != 0)
		//				f.m_values[dim.m_index] = dim.m_value;
		//		}
		//	}

		//	resetDimValue();
		//	return f;
		//}

		//Frame createFrame(int index)
		//{
		//	Frame f = m_frames[index];
		//	//f.m_values.resize(m_dims);
		//	for(unsigned int i = 0; i < m_joints.size(); ++i)
		//	{
		//		Joint* joint = m_joints[i];
		//		for(int j = 0; j < joint->m_dof; ++j)
		//		{
		//			Dim& dim = joint->m_dims[j];
		//			//if(dim.m_value != 0)
		//				f.m_values[dim.m_index] = dim.m_value;
		//		}
		//		if(joint->m_name == "Hips"||joint->m_name == "RightHip"||joint->m_name == "RightKnee"||joint->m_name == "RightAnkle"||joint->m_name == "RightToe"||joint->m_name == "LeftHip"||joint->m_name == "LeftKnee"||joint->m_name == "LeftAnkle"||joint->m_name == "LeftToe")
		//		{
		//			for(int j = 0; j < joint->m_dof; ++j)
		//			{
		//				Dim& dim = joint->m_dims[j];
		//				f.m_values[dim.m_index] = 0;
		//			}
		//		}
		//	}

		//	resetDimValue();
		//	return f;
		//}

		//void resetDimValue()
		//{
		//	for(unsigned int i = 0; i < m_joints.size(); ++i)
		//	{
		//		Joint* joint = m_joints[i];
		//		for(int j = 0; j < joint->m_dof; ++j)
		//		{
		//			Dim& dim = joint->m_dims[j];
		//			dim.m_value = 0;
		//		}
		//	}
		//}

		//
		Skeleton m_skeleton;
		std::vector<Frame> m_frames;
		float m_frametime; 
		int m_frameNb;
	public:
		BVH(){}
		BVH(BVH& bvh)
		{
			m_frames = bvh.m_frames;
			m_frameNb = bvh.m_frameNb;
			m_frametime = bvh.m_frametime;
			m_skeleton = bvh.m_skeleton;
		}

		bool loadFromBVHFile(const std::string& filepath);
		bool saveToBVHFile(const std::string& filepath);
		void changeOrderToZYX();

		bool loadTextFile(const std::string& filepath);
		bool saveTextFile(const std::string& filepath);

	private:
		void trimString( std::string& string);
		void read(std::istream& in);
		void readJoint(std::istream& in, const std::string& name);
		void readEnd(std::istream& in, const std::string& name);
		void readLine(std::istream& in, std::string& line);
		void readFrames(std::istream& in);

		void write(std::ostream& out);
		std::string m_filepath;
		std::stack<int> m_index;
	};

